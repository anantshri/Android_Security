#include<stdio.h>
#include<stdlib.h>
#include<elf.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dlfcn.h>



int getGOTOffset(const char * module_name,const char * symbol_name,int *memcpyOffset)
{
    int fd = open(module_name, O_RDONLY);    
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)malloc(sizeof(Elf32_Ehdr));
    read(fd, ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr);
    uint32_t shdr_base = ehdr -> e_shoff;
    uint16_t shnum = ehdr -> e_shnum;
    uint32_t shstr_base = shdr_base + ehdr -> e_shstrndx * sizeof(Elf32_Shdr);
    Elf32_Shdr *shstr = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr));
    lseek(fd, shstr_base, SEEK_SET);
    read(fd, shstr, sizeof(Elf32_Shdr));

    char *shstrtab = (char *)malloc(shstr -> sh_size);
    lseek(fd, shstr -> sh_offset, SEEK_SET);
    read(fd, shstrtab, shstr -> sh_size);
    Elf32_Shdr *shdr = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr));
    lseek(fd, shdr_base, SEEK_SET);
    uint16_t i;
    char *str = NULL;
    Elf32_Shdr *relplt_shdr = (Elf32_Shdr *) malloc(sizeof(Elf32_Shdr));
    Elf32_Shdr *dynsym_shdr = (Elf32_Shdr *) malloc(sizeof(Elf32_Shdr));
    Elf32_Shdr *dynstr_shdr = (Elf32_Shdr *) malloc(sizeof(Elf32_Shdr));
    Elf32_Shdr *got_shdr = (Elf32_Shdr *) malloc(sizeof(Elf32_Shdr));
    for(i = 0; i < shnum; ++i) {
        read(fd, shdr, sizeof(Elf32_Shdr));
        str = shstrtab + shdr -> sh_name;
        if(strcmp(str, ".dynsym") == 0)
            memcpy(dynsym_shdr, shdr, sizeof(Elf32_Shdr));
        else if(strcmp(str, ".dynstr") == 0)
            memcpy(dynstr_shdr, shdr, sizeof(Elf32_Shdr));
        else if(strcmp(str, ".got") == 0)
            memcpy(got_shdr, shdr, sizeof(Elf32_Shdr));
        else if(strcmp(str, ".rel.plt") == 0)
            memcpy(relplt_shdr, shdr, sizeof(Elf32_Shdr));
    }


    //读取字符表
    char *dynstr = (char *) malloc(sizeof(char) * dynstr_shdr->sh_size);
    lseek(fd, dynstr_shdr->sh_offset, SEEK_SET);
    if((size_t)read(fd, dynstr, dynstr_shdr->sh_size) != dynstr_shdr->sh_size)
        return -1;

    //读取符号表
    Elf32_Sym *dynsymtab = (Elf32_Sym *) malloc(dynsym_shdr->sh_size);
    //printf("dynsym_shdr->sh_size\t0x%x\n", dynsym_shdr->sh_size);
    lseek(fd, dynsym_shdr->sh_offset, SEEK_SET);
    if((size_t)read(fd, dynsymtab, dynsym_shdr->sh_size) != dynsym_shdr->sh_size)
        return -1;

    //读取重定位表
    Elf32_Rel *rel_ent = (Elf32_Rel *) malloc(sizeof(Elf32_Rel));
    lseek(fd, relplt_shdr->sh_offset, SEEK_SET);
    if((size_t)read(fd, rel_ent, sizeof(Elf32_Rel)) != sizeof(Elf32_Rel))
        return -1;

    //获取指定符号在got表的偏移地址
    int offset=0;
    //const char *symbol_name = "memcpy";
    for (i = 0; i < relplt_shdr->sh_size / sizeof(Elf32_Rel); i++) {
        uint16_t ndx = ELF32_R_SYM(rel_ent->r_info);
        //printf("ndx = %d, str = %s", ndx, dynstr + dynsymtab[ndx].st_name);
        if (strcmp(dynstr + dynsymtab[ndx].st_name, symbol_name) == 0) {
           // printf("符号%s在got表的偏移地址为: 0x%x\n", symbol_name, rel_ent->r_offset);
            offset = rel_ent->r_offset;
            *memcpyOffset = offset;
            break;
        }
        if(read(fd, rel_ent, sizeof(Elf32_Rel)) != sizeof(Elf32_Rel)) {
            printf("获取符号%s的重定位信息失败\n", symbol_name);
            return -1;
        }
    }
    return 0;
}

void *getProcAddr(const char *moduleName,const char *funcName)
{

    void *handle;
    void *retAddress = NULL;
    const char *error;
    handle = dlopen (moduleName, RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    retAddress = dlsym(handle, funcName);
    if (retAddress== NULL)  {
        fputs(dlerror(), stderr);
        exit(1);
    }
    //printf ("address of %s is %p\n", funcName, retAddress);
    dlclose(handle);
    return retAddress;
}

void *getModuleBase(const char *moduleName)
{

    void *handle = dlopen (moduleName, RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        return NULL;
    }
    dlclose(handle);
    //soinfo + 0x8c = module base
    //raise(SIGINT);
    return *(void **)((char *)(handle)+0x8c);
}

unsigned int getOffset(const char *moduleName,const char *funcName)
{

    void *moduleBase = getModuleBase(moduleName);
    void *funcAddress = getProcAddr(moduleName,funcName);
    return (unsigned int)funcAddress -1- (unsigned int)moduleBase;
}

int fakemain()
{

    int offset = 0;
    getGOTOffset("/system/lib/libui.so","memcpy",&offset);
    //printf("offset is %x\n",offset);
    
    getProcAddr("libc.so","system");
    //printf("module base is %p\n",getModuleBase("libc.so"));

    //printf("offset is %x\n", getOffset("libc.so","system"));
    //printf("offset is %x\n", getOffset("libc.so","memcpy"));
    
    return 0;
}
