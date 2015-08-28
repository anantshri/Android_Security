#include<stdio.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dlfcn.h>
#include<elf.h>
typedef void (*NOARGFUN)();
void run_code(char *code,void* dlopen_f);
void *getModuleBase(const char *moduleName);
/*void *getModuleBase(const char *moduleName)
{

    void *handle = dlopen (moduleName, RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        return NULL;
    }   
    dlclose(handle);
    //soinfo + 0x8c = module base
    //    //raise(SIGINT);
    return *(void **)((char *)(handle)+0x8c);
}*/
void* relocate(void *sobase){
#define OFFSET2VA(offset) ((uint32_t)(sobase)+(uint32_t)(offset))
    Elf32_Ehdr *ehdr = (Elf32_Ehdr*)sobase;
    Elf32_Shdr *shstr = (Elf32_Shdr*)OFFSET2VA(ehdr->e_shoff + ehdr->e_shstrndx*sizeof(Elf32_Shdr));
    Elf32_Shdr *first_sh = (Elf32_Shdr*)OFFSET2VA(ehdr->e_shoff);
    char *shstrtab = (char*)OFFSET2VA(shstr->sh_offset);
    char *gotSymbol = (char*)malloc(2000);
    Elf32_Shdr *relplt_shdr = NULL;
    Elf32_Shdr *reldyn_shdr = NULL;
    Elf32_Shdr *dynsym_shdr = NULL; 
    Elf32_Shdr *dynstr_shdr = NULL; 
    Elf32_Shdr *got_shdr = NULL;
    Elf32_Shdr *initarray_shdr = NULL;
    Elf32_Shdr *datarelro_shdr = NULL;
    for(int i=0;i<ehdr->e_shnum;i++){
        char *str = shstrtab + first_sh[i].sh_name; 
        if(strcmp(str, ".dynsym") == 0)
            dynsym_shdr=&first_sh[i];
        else if(strcmp(str, ".dynstr") == 0)
            dynstr_shdr=&first_sh[i];
        else if(strcmp(str, ".got") == 0)
            got_shdr=&first_sh[i];
        else if(strcmp(str, ".rel.plt") == 0)
            relplt_shdr=&first_sh[i];
        else if(strcmp(str, ".rel.dyn") == 0)
            reldyn_shdr=&first_sh[i];
        else if(strcmp(str, ".init_array") == 0)
            initarray_shdr=&first_sh[i];
        else if(strcmp(str, ".data.rel.ro") == 0)
            datarelro_shdr=&first_sh[i];
    }
    char *dynstr = (char*)OFFSET2VA(dynstr_shdr->sh_offset);
    Elf32_Sym *dynsymtab = (Elf32_Sym*)OFFSET2VA(dynsym_shdr->sh_offset);
    const uint32_t len = 4096*5;
    char *buffer = (char *)malloc(len);
    memset(buffer,0,len);
    uint32_t j=0;
    Elf32_Rel *rel_ent= (Elf32_Rel*)OFFSET2VA(relplt_shdr->sh_offset);
    for (uint32_t i=0; i < relplt_shdr->sh_size / sizeof(Elf32_Rel)&&j<len; i++) {
        uint16_t ndx = ELF32_R_SYM(rel_ent[i].r_info);
        char *name = dynstr + dynsymtab[ndx].st_name;
        int  offset = rel_ent[i].r_offset;
        if(j+4>len)
            break;
        *(int*)(buffer+j)=offset;
        j+=4;
        if(j+4>len)
            break;
        *(int*)(buffer+j)=dynsymtab[ndx].st_value;
        j+=4;
        if(j+strlen(name)>len)
            break;
        strcpy(buffer+j,name);
        j+=strlen(name)+1;
    }
    rel_ent= (Elf32_Rel*)OFFSET2VA(reldyn_shdr->sh_offset);
    for (uint32_t i=0; i < reldyn_shdr->sh_size / sizeof(Elf32_Rel)&&j<len; i++) {
        uint16_t ndx = ELF32_R_SYM(rel_ent[i].r_info);
        char *name = dynstr + dynsymtab[ndx].st_name;
        int  offset = rel_ent[i].r_offset;
        if(j+4>len)
            break;
        *(int*)(buffer+j)=offset;
        j+=4;
        if(j+4>len)
            break;
        *(int*)(buffer+j)=dynsymtab[ndx].st_value;
        j+=4;
        if(j+strlen(name)>len)
            break;
        strcpy(buffer+j,name);
        j+=strlen(name)+1;
    }
    *(int*)(buffer+j)=0xcccccccc;
    j+=4;
    if(datarelro_shdr!=NULL){
        int *datarelro = (int*)OFFSET2VA(datarelro_shdr->sh_offset);
        *(int*)(buffer+j)=datarelro_shdr->sh_addr;
        j+=4;
        for(uint32_t i=0;i<datarelro_shdr->sh_size/4;i++){
            *(int*)(buffer+j)=datarelro[i];
            j+=4;
        }
    }
    //forget the overflow
    *(int*)(buffer+j)=0xcccccccc;
    j+=4;
    if(initarray_shdr!=NULL){
        void **initarray=(void**)OFFSET2VA(initarray_shdr->sh_offset);
        uint32_t init_array_len = initarray_shdr->sh_size/4;
        for(uint32_t i=0;i<init_array_len;i++){
            *(void**)(buffer+j)=initarray[i];
            j+=4;
            printf("init array %d at %p\n",i,initarray[i]);
        }
    }
    //handle data.rel.ro
    *(int*)(buffer+j)=0xcccccccc;
    return buffer;
    //printf("section name is %p,%s,%d\n",section_name,section_name,first_sh[i].sh_name);

}

void *get_so_buffer(const char *so_name){

    int so_fd = open(so_name,O_RDONLY);//"/data/local/tmp/testso.so"
    struct stat buf;
    fstat(so_fd, &buf);
    int size = buf.st_size;
    void *so_file=malloc(size);
    read(so_fd,so_file,size);
    char *buffer = (char*)relocate(so_file);
    close(so_fd);

    void *so_handle = dlopen(so_name,RTLD_LAZY);
    void *so_start = dlsym(so_handle,"so_main");
    void *so_base = *(void**)((int)so_handle+0x8c);
    uint32_t so_size = *(uint32_t*)((int)so_handle+0x90);
    mprotect (so_base, so_size, PROT_READ|PROT_WRITE|PROT_EXEC);
    printf("base is %p,size is %d\n",so_base,so_size); 
    const uint32_t len = 4096*5;
    char *code = (char*)malloc(so_size+len+8);
    *(int*)code = so_size+len+8;
    *(int*)(code+4) = (int)so_start-(int)so_base;

    memcpy(code+8,buffer,len);
    free(buffer);
    memcpy(code+len+8,so_base,so_size);
    dlclose(so_handle);
    return code;


}

int runsc_main(){
    const char *so_name = "/data/local/tmp/libexpsur.so";//testso.so
    char *code = (char*)get_so_buffer(so_name); 
    mprotect (code, *(int*)code, PROT_READ|PROT_WRITE|PROT_EXEC);
    run_code(code,(void*)dlopen);
    free(code);
    return 0;
}
