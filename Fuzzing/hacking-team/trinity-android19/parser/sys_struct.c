/* Struct to be analyzed on arm architecture */
/* We removed obsolete syscall parameters and output arguments */


struct timespec {
  __kernel_time_t tv_sec; 
  long            tv_nsec;
};

struct timeval {
  __kernel_time_t         tv_sec;
  __kernel_suseconds_t    tv_usec;
};

struct timezone {
  int     tz_minuteswest;
  int     tz_dsttime;
};

struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;   
};

struct itimerval {
  struct timeval it_interval;
  struct timeval it_value; 
};

struct sigevent {
  sigval_t sigev_value;
  int sigev_signo;
  int sigev_notify;
  union {
    int _pad[SIGEV_PAD_SIZE];
    int _tid;
    
    struct {
      void (*_function)(sigval_t);
      void *_attribute;	
    } _sigev_thread;
  } _sigev_un;
} sigevent_t;


struct linux_dirent {
  unsigned long	d_ino;
  unsigned long	d_off;
  unsigned short d_reclen;
  char		d_name[1];
};

// Da verificare (ABI)
struct stat64 {
  unsigned long long	st_dev;
  unsigned char __pad1[2];
  
  /*#define STAT64_HAS_BROKEN_ST_INO	1*/
  unsigned long			__st_ino;
  
  unsigned int			st_mode;
  unsigned int			st_nlink;
  
  unsigned long			st_uid;
  unsigned long			st_gid;
  
  unsigned long long		st_rdev;
  unsigned char __pad3[2];
  
  long long     st_size;
  unsigned long st_blksize;
  
  unsigned long __pad4;		/* future possible st_blocks high bits */
  unsigned long st_blocks;	/* Number 512-byte blocks allocated. */
  
  unsigned long st_atime;
  unsigned long st_atime_nsec;
  
  unsigned long st_mtime;
  unsigned long st_mtime_nsec;
  
  unsigned long st_ctime;
  unsigned long st_ctime_nsec;
  
  unsigned long long	st_ino;
};



struct rlimit64 {
  __u64 rlim_cur;
  __u64 rlim_max;
};


struct msghdr {
  void   *msg_name;
  int     msg_namelen;
  struct iovec *msg_iov;
  __kernel_size_t msg_iovlen;
  void *msg_control;
  __kernel_size_t msg_controllen;
  unsigned msg_flags;
};


struct mmsghdr {
  struct msghdr   msg_hdr;
  unsigned        msg_len;
};


struct tms {
  __kernel_clock_t tms_utime;
  __kernel_clock_t tms_stime;
  __kernel_clock_t tms_cutime;
  __kernel_clock_t tms_cstime;
};


struct epoll_event {
  __u32 events;
  __u64 data;
} EPOLL_PACKED;

struct getcpu_cache {
  unsigned long blob[32];
};

struct rlimit {
  unsigned long	rlim_cur;
  unsigned long	rlim_max;
};


struct iocb {
  /* these are internal to the kernel/libc. */
  __u64	aio_data;    /* data to be returned in event's data */
  __u32	PADDED(aio_key, aio_reserved1);
  /* the kernel sets aio_key to the req # */
  
  /* common fields */
  __u16	  aio_lio_opcode;	/* see IOCB_CMD_ above */
  __s16	  aio_reqprio;
  __u32	  aio_fildes;
  
  __u64	  aio_buf;
  __u64	  aio_nbytes;
  __s64	  aio_offset;
  
  /* extra parameters */
  __u64	 aio_reserved2;	/* TODO: use this for a (struct sigevent *) */
  
  /* flags for the "struct iocb" */
  __u32	 aio_flags;
  
  __u32	aio_resfd;  
}

/* Aready done by trinity
struct perf_event_attr {
  
  __u32 type;
  __u32 size;
  __u64 config;
  
  union {
    __u64 sample_period;
    __u64 sample_freq;
  };
  
  __u64 sample_type;
  __u64 read_format;
  
  __u64 disabled       :  1, 
    inherit       :  1, 
    pinned       :  1, 
    exclusive      :  1, 
    exclude_user   :  1, 
    exclude_kernel :  1, 
    exclude_hv     :  1, 
    exclude_idle   :  1, 
    mmap           :  1, 
    comm       :  1, 
    freq           :  1, 
    inherit_stat   :  1, 
    enable_on_exec :  1, 
    task           :  1, 
    watermark      :  1, 
    precise_ip     :  2, 
    mmap_data      :  1, 
    sample_id_all  :  1, 
    
    exclude_host   :  1, 
    exclude_guest  :  1, 
    __reserved_1   : 43;
  
  union {
    __u32 wakeup_events;  
    __u32 wakeup_watermark; 
  };
  
  __u32 bp_type;
  union {
    __u64 bp_addr;
    __u64 config1; 
  };
  union {
    __u64 bp_len;
    __u64 config2; 
  };
  __u64 branch_sample_type; 
  }; */


struct new_utsname {
  char sysname[65]; 
  char nodename[65];
  char release[65];
  char version[65];
  char machine[65];
  char domainname[65];
};


struct stat {
  unsigned short st_dev;
  unsigned short __pad1;
  unsigned long st_ino;
  unsigned short st_mode;
  unsigned short st_nlink;
  unsigned short st_uid;
  unsigned short st_gid;
  unsigned short st_rdev;
  unsigned short __pad2;
  unsigned long  st_size;
  unsigned long  st_blksize;
  unsigned long  st_blocks;
  unsigned long  st_atime;
  unsigned long  __unused1;
  unsigned long  st_mtime;
  unsigned long  __unused2;
  unsigned long  st_ctime;
  unsigned long  __unused3;
  unsigned long  __unused4;
  unsigned long  __unused5;
};


struct linux_dirent64 {
  u64 d_ino;
  s64 d_off;
  unsigned shortd_reclen;
  unsigned chard_type;
  char d_name[0];
};

struct linux_dirent {
  unsigned longd_ino;
  unsigned longd_off;
  unsigned shortd_reclen;
  char d_name[1];
};


// Controllare (ABI)
struct sembuf {
  unsigned short  sem_num; /* semaphore index in array */
  short sem_op; /* semaphore operation */
  short sem_flg; /* operation flags */
};


struct statfs64 {
  __statfs_word f_type;
  __statfs_word f_bsize;
  __u64 f_blocks;
  __u64 f_bfree;
  __u64 f_bavail;
  __u64 f_files;
  __u64 f_ffree;
  __kernel_fsid_t f_fsid;
  __statfs_word f_namelen;
  __statfs_word f_frsize;
  __statfs_word f_flags;
  __statfs_word f_spare[4];
} ARCH_PACK_STATFS64;


struct shmid_ds {
  struct ipc_perm shm_perm; /* operation perms */
  int shm_segsz; /* size of segment (bytes) */
  __kernel_time_tshm_a time; /* last attach time */
  __kernel_time_tshm_d time; /* last detach time */
  __kernel_time_tshm_c time; /* last change time */
  __kernel_ipc_pid_tshm_c pid; /* pid of creator */
  __kernel_ipc_pid_tshm_l pid; /* pid of last operator */
  unsigned shortshm_nattch; /* no. of current attaches */
  unsigned short shm_unused; /* compatibility */
  void *shm_unused2; /* ditto - used by DIPC */
  void*shm_unused3; /* unused */
};


struct pollfd {
  int fd;
  short events;
  short revents;
};

struct msgbuf {
  long mtype;         /* type of message */
  char mtext[1];      /* message text */
};


struct file_handle {
  __u32 handle_bytes;
  int handle_type;
  /* file identifier */
  unsigned char f_handle[0];
};


struct robust_list_head {
  struct robust_list list;
  long futex_offset;
  struct robust_list *list_op_pending;
};

struct robust_list {
  struct robust_list *next;
};

struct rusage {
  struct timeval ru_utime; /* user time used */
  struct timeval ru_stime; /* system time used */
  long ru_maxrss; /* maximum resident set size */
  long ru_ixrss; /* integral shared memory size */
  long ru_idrss; /* integral unshared data size */
  long ru_isrss; /* integral unshared stack size */
  long ru_minflt; /* page reclaims */
  long ru_majflt; /* page faults */
  long ru_nswap; /* swaps */
  long ru_inblock; /* block input operations */
  long ru_oublock; /* block output operations */
  long ru_msgsnd; /* messages sent */
  long ru_msgrcv; /* messages received */
  long ru_nsignals; /* signals received */
  long ru_nvcsw; /* voluntary context switches */
  long ru_nivcsw; /* involuntary " */
};


struct ustat {
  __kernel_daddr_t        f_tfree;
  __kernel_ino_t          f_tinode;
  char                    f_fname[6];
  char                    f_fpack[6];
};


struct timex {
  unsigned int modes; /* mode selector */
  long offset; /* time offset (usec) */
  long freq; /* frequency offset (scaled ppm) */
  long maxerror; /* maximum error (usec) */
  long esterror; /* estimated error (usec) */
  int status; /* clock command/status */
  long constant; /* pll time constant */
  long precision; /* clock precision (usec) (read only) */
  long tolerance; 
  struct timeval time; /* (read only, except for ADJ_SETOFFSET) */
  long tick; /* (modified) usecs between clock ticks */

  long ppsfreq;           /* pps frequency (scaled ppm) (ro) */
  long jitter;            /* pps jitter (us) (ro) */
  int shift;              /* interval duration (s) (shift) (ro) */
  long stabil;            /* pps stability (scaled ppm) (ro) */
  long jitcnt;            /* jitter limit exceeded (ro) */
  long calcnt;            /* calibration intervals (ro) */
  long errcnt;            /* calibration errors (ro) */
  long stbcnt;            /* stability limit exceeded (ro) */

  int tai; /* TAI offset (ro) */

  int  :32; int  :32; int  :32; int  :32;
  int  :32; int  :32; int  :32; int  :32;
  int  :32; int  :32; int  :32;
};

struct statfs {
  long f_type;
  /*#define f_fstyp f_type*/
  long f_bsize;
  long f_frsize; /* Fragment size - unsupported */
  long f_blocks;
  long f_bfree;
  long f_files;
  long f_ffree;
  long f_bavail;

  /* Linux specials */
  __kernel_fsid_tf_f sid;
  long f_namelen;
  long f_flags;
  long f_spare[5];
};


struct msqid_ds {
  struct ipc_perm msg_perm;
  struct msg *msg_first; /* first message on queue,unused  */
  struct msg *msg_last; /* last message in queue,unused */
  __kernel_time_t msg_stime; /* last msgsnd time */
  __kernel_time_t msg_rtime; /* last msgrcv time */
  __kernel_time_t msg_ctime; /* last change time */
  unsigned long  msg_lcbytes; /* Reuse junk fields for 32 bit */
  unsigned long  msg_lqbytes; /* ditto */
  unsigned short msg_cbytes; /* current number of bytes on queue */
  unsigned short msg_qnum; /* number of messages in queue */
  unsigned short msg_qbytes; /* max number of bytes on queue */
  __kernel_ipc_pid_t msg_lspid; /* pid of last msgsnd */
  __kernel_ipc_pid_t msg_lrpid; /* last receive pid */
};

struct ipc_perm
{
  __kernel_key_t key;
  __kernel_uid_t uid;
  __kernel_gid_t gid;
  __kernel_uid_t cuid;
  __kernel_gid_t cgid;
  __kernel_mode_t mode; 
  unsigned shortseq;
};

/* NPE message structure */
struct msg {
  u8 byte3, byte2, eth_id, cmd;
  u8 byte7, byte6, byte5, byte4;
};


struct __sysctl_args {
  int *name;
  int nlen;
  void *oldval;
  size_t *oldlenp;
  void *newval;
  size_t newlen;
  unsigned long __unused[4];
};

struct mq_attr {
  long mq_flags;      /* message queue flags*/
  long mq_maxmsg;     /* maximum number of messages*/
  long mq_msgsize;    /* maximum message size*/
  long mq_curmsgs;    /* number of messages currently queued*/
  long __reserved[4]; /* ignored for input, zeroed for output */
};

struct io_event {
  __u64 data;   /* the data field from the iocb */
  __u64 obj;    /* what iocb this event came from */
  __s64 res;    /* result code for this event */
  __s64 res2;   /* secondary result */
};


struct sched_param {
  int sched_priority;
};


struct kexec_segment {
  void *buf;
  size_t bufsz;
  unsigned long mem;      /* User space sees this as a (void *) ... */
  size_t memsz;
};
