#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include <sys/ptrace.h>
#include<sys/user.h>
#include<fcntl.h>
#include<dlfcn.h>
#include<elf.h>
#include<android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "exploit", __VA_ARGS__)
const int long_size = sizeof(long);
void getdata2(pid_t child, long addr,
        char *str, int len)
{   char *laddr;
    int i, j;
    union u {
        long val;
        char chars[long_size];
    }data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j) {
        data.val = ptrace(PTRACE_PEEKDATA, child,
                addr + i * 4, NULL);
        memcpy(laddr, data.chars, long_size);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA, child,
                addr + i * 4, NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '\0';
}
void putdata(pid_t child, long addr,
        char *str, int len)
{   char *laddr;
    int i, j;
    union u {
        long val;
        char chars[long_size];
    }data;
    i = 0;
    j = len / long_size;
    laddr = str;
    while(i < j) {
        memcpy(data.chars, laddr, long_size);
        ptrace(PTRACE_POKEDATA, child,
                addr + i * 4, data.val);
        ++i;
        laddr += long_size;
    }
    j = len % long_size;
    if(j != 0) {
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKEDATA, child,
                addr + i * 4, data.val);
    }
}
void error_exit(const char* msg)
{
    perror(msg);
    _exit(1);
}

void getdata(pid_t pid, long addr, long* str, const int word_count)
{
    for(int i=0;i<word_count;i++){
        long data = ptrace(PTRACE_PEEKTEXT, pid, addr+i*4, NULL);
        /*if(data==-1){
          error_exit("peek data fail\n");
          }*/
        str[i]=data;
    }
}

__attribute__((naked)) void 
switch_context(__attribute__((unused)) void *user_reges,__attribute__((unused)) void *jump_to){
    asm ("push {r4-r12}");
    asm ("mov r11,sp");
    asm ("push {r11}");

    asm ("mov sp,r0");
    //asm ("mov r12,r1");
    asm ("pop {r0-r10}");
    asm ("add sp,#8");
    asm ("pop {r11}");//get sp
    asm ("add sp,#4");
    asm ("pop {r12}");//get pc
    asm ("mov sp,r11");
    asm ("mov r11,#16");
    asm ("msr cpsr,r11");
    asm ("mov r11,#0");
    //asm ("bkpt #1");
    asm ("blx r12");

    asm ("pop {r0}");
    asm ("mov sp,r0");
    asm ("pop {r4-r12}");
}


typedef int (*bbmain_t)(int argc , const char *argv[]);
typedef int (*noargfun_t)();

/*0x1fa430 <re_comp_buf+24>:    0x00000000  0x00000000  0xbeb1cad0  0x00000000
 * 0x1faf70 <startp.13743>: 0x00000000  0x00000000  0x0163d000  0x00000000
 * Start Addr   End Addr       Size     Offset objfile
 * 0x8000   0x1f1000   0x1e9000        0x0 /data/local/tmp/busybox
 * 0x1f8000   0x1fa000     0x2000   0x1e8000 /data/local/tmp/busybox
 * 0x1fa000   0x1fe000     0x4000        0x0 
 * 0x161b000  0x163d000    0x22000        0x0 [heap]
 * 0xbeafc000 0xbeb1d000    0x21000        0x0 [stack]
 * 0xffff0000 0xffff1000     0x1000        0x0 [vectors]
 */
uint32_t region1_start = 0x8000;
uint32_t region2_start = 0x1f8000;
uint32_t region3_start = 0x1fa000;
uint32_t region1_end = 0x1f1000;
uint32_t region2_end= 0x1fa000;
uint32_t region3_end= 0x1fe000;
uint32_t region1_size = region1_end-region1_start;
uint32_t region2_size = region2_end-region2_start;
uint32_t region3_size = region3_end-region2_end;
long stack_base=0;
long heap_base=0;
uint32_t stack_size = 0x21000;
uint32_t heap_size = 0x22000;
bbmain_t bbmain = (bbmain_t)0xd038;
struct user_regs regs;
uint32_t bb_buffer_len = region1_size+region2_size+region3_size+stack_size+heap_size+8+sizeof(regs);

int get_mapped_bb(){

    void *region1=mmap((void*)region1_start,region1_end-region1_start,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    void *region2=mmap((void*)region2_start,region2_end-region2_start,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    void *region3=mmap((void*)region2_end,region3_end-region2_end,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    uint32_t stack_base_offset = 0x1fa438;
    uint32_t heap_base_offset = 0x1faf78;
    long bkcode=0xbe01be01;
    noargfun_t _start = (noargfun_t)0xc460;
    noargfun_t _init = (noargfun_t)0x8114;
    noargfun_t _initarray = (noargfun_t)0xc57c;
    if(region1!=(void*)region1_start||region2!=(void*)region2_start||region3!=(void*)region2_end){
        printf("can't relocate busybox\n");
        return -1;
    }
    printf("%p,%p,%p\n",region1,region2,region3);
    int pid = fork();
    int status;
    if(pid==0){
        printf("in clild\n");
        /* Allow tracing of this process */
        if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
            perror("ptrace");
            return -1;
        }
        //long argue list to occupy the place so we can replace
        char *const argv[]={(char*)"xx",(char*)"xx",(char*)"xx",(char*)"xx",(char*)"xx",(char*)"xx",(char*)"xx",(char*)"xx",(char*)0};
        execv("/data/local/tmp/busybox",argv);
    }else{
        printf("in father %d\n",pid);
        /*long ret=ptrace(PTRACE_ATTACH, pid, NULL, NULL); 
          if(ret==-1){
          error_exit("attach fail\n");
          }*/
        waitpid(pid, &status,0);
        if (WIFEXITED(status)){
            error_exit("ptrace\n");
        }
        //char backup[4];
        /* Copy instructions into a backup variable */
        //getdata(pid, (long)bbmain, (long*)backup, 1);
        /* Put the breakpoint */
        //putdata(pid, (long)bbmain, (char*)&bkcode, 4);
        /* Let the process continue and execute
         *        the int 3 instruction */
        //ptrace(PTRACE_CONT, pid, NULL, NULL);

        //read regs
        //waitpid(pid, &status,0);
        ptrace(PTRACE_GETREGS, pid, 0, &regs);
        if (WIFEXITED(status)){
            error_exit("ptrace\n");
        }
        if (WIFSTOPPED(status)){
            //getdata(pid,heap_base_offset,&heap_base,1);
            stack_base = (regs.uregs[13]&0xfffff000)+0x1000-stack_size;
            //heap_base = heap_base-heap_size;
            printf("stack base is %.8x, heap base is %.8x\n",(unsigned int)stack_base,(uint32_t)heap_base);
            /*char buffer[200];
            sprintf(buffer,"su -c cat /proc/%d/maps",pid);
            system(buffer);*/
            long stack=(long)mmap((void*)stack_base,stack_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
            //long heap=(long)mmap((void*)heap_base,heap_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
            if(stack!=stack_base){//||heap!=heap_base
                printf("can't relocate busybox\n");
                return -1;
            }

            getdata(pid,region1_start ,(long*)region1,region1_size/4);
            getdata(pid,region2_start ,(long*)region2,region2_size/4);
            getdata(pid,region2_end ,(long*)region3,region3_size/4);
            getdata(pid,stack_base,(long*)stack_base,stack_size/4);
            //getdata(pid,heap_base,(long*)heap_base,heap_size/4);
            //*(long*)bbmain=*(long*)backup;
        }
        printf("go to detach\n");
        ptrace(PTRACE_CONT, pid, NULL, NULL);
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        waitpid(pid, &status, 0);
        //switch_context(&regs,(void*)bbmain);
        //_start();
        //_init();
        //_initarray();
    }
    return 0;
}



char *pack_data(){
    get_mapped_bb();
    char *buffer = (char*)malloc(bb_buffer_len);
    char *p=buffer;
    *(uint32_t*)p=stack_base;
    p+=4;
    *(uint32_t*)p=heap_base;
    p+=4;
    memcpy(p,(void*)region1_start,region1_size);
    p+=region1_size;
    memcpy(p,(void*)region2_start,region2_size);
    p+=region2_size;
    memcpy(p,(void*)region3_start,region3_size);
    p+=region3_size;
    memcpy(p,(void*)stack_base,stack_size);
    p+=stack_size;
    //memcpy(p,(void*)heap_base,heap_size);
    //p+=heap_size;
    memcpy(p,&regs,sizeof(regs));
    p+=sizeof(regs);
    return buffer;
}
void unpack_data(char *buffer){
    char *p = buffer;
    stack_base=*(uint32_t*)p;
    p+=4;
    heap_base=*(uint32_t*)p;
    p+=4;
    void *region1=mmap((void*)region1_start,region1_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    memcpy((void*)region1_start,p,region1_size);
    p+=region1_size;
    void *region2=mmap((void*)region2_start,region2_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    memcpy((void*)region2_start,p,region2_size);
    p+=region2_size;
    void *region3=mmap((void*)region3_start,region3_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    memcpy((void*)region3_start,p,region3_size);
    p+=region3_size;

    long stack=(long)mmap((void*)stack_base,stack_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    memcpy((void*)stack_base,p,stack_size);
    p+=stack_size;
    //long heap=(long)mmap((void*)heap_base,heap_size,PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
    //memcpy((void*)heap_base,p,heap_size);
    //p+=heap_size;
    memcpy(&regs,p,sizeof(regs));
    p+=sizeof(regs);
}

void unmapall(){
    munmap((void*)region1_start,region1_size);
    munmap((void*)region2_start,region2_size);
    munmap((void*)region3_start,region3_size);
    munmap((void*)stack_base,stack_size);
    //munmap((void*)heap_base,heap_size);
    memset(&regs,0,sizeof(regs));
}
void* exec_bbcmd(const char *argv[],uint32_t *out_len){
        int filedes[2];
        int status;
        if (pipe(filedes) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t com_pid = fork();
        if (com_pid == -1) {
            perror("fork");
            exit(1);
        } else if (com_pid == 0) {
            if(argv[0]==0)
                LOGE("fuck ,error\n");
                
            LOGE("start execute bbmain %s,%p,%p,%p",argv[0],argv[1],argv[2],argv[3]);
            //sleep(15);
            //bbmain(10,argv);
            //replace the argues
            uint32_t *sp=(uint32_t*)regs.uregs[13];
            uint32_t *pargv = sp+1;
            uint32_t i=0;
            for(i=0;argv[i]!=0;i++){
                pargv[i]=(uint32_t)argv[i];
            }
            pargv[i]=0;
            sp[0]=i;
            LOGE("regs %lx,%lx,%lx,%lx\n",regs.uregs[0],regs.uregs[1],regs.uregs[2],regs.uregs[3]);
            fflush(stdout);
            fflush(stderr);
            close(STDERR_FILENO); 
            close(STDOUT_FILENO); 
            while ((dup2(filedes[1], STDERR_FILENO) == -1) && (errno == EINTR)) {}
            while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
            close(filedes[1]);
            close(filedes[0]);
            switch_context(&regs,NULL);
            fflush(stdout);
            LOGE("end execute bbmain");
            _exit(1);
        }else{
            
            LOGE("child pid is %d",com_pid);
            close(filedes[1]);
            char *buffer = (char *)malloc(8192*4);
            ssize_t count=0;
            memset(buffer,0,8192);
            while((count=read(filedes[0], buffer, 8192*4))>0) {
                   LOGE("%d,%s end",count,(char*)buffer);
                   //printf("%s",buffer);
                   //memset(buffer,0,8192);
                   *out_len = count; }
            LOGE("exit looop");
            close(filedes[0]);
            waitpid(com_pid, &status, 0);
            LOGE("child died");
            return buffer;
        }
}
