#include "bbshell.h"
#include <stdio.h>
int main(){
    char *buffer=pack_data();
    unmapall();
    unpack_data(buffer);
    char *out = NULL;
    //const char *argv[]={"cat","/data/local/tmp/a.txt",NULL};
    //out=(char*)exec_bbcmd(argv);
    //printf("%s",out);
    //free(out);
    uint32_t out_len;
    const char *argv2[]={"ls","/data",NULL};
    out=(char*)exec_bbcmd(argv2,&out_len);
    printf("%s",out);
    free(out);
    const char *argv3[]={"ifconfig",NULL};
    out=(char*)exec_bbcmd(argv3,&out_len);
    printf("%s",out);
    free(out);
    free(buffer);
}
