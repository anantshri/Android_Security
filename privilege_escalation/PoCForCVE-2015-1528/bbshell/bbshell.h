#include<sys/types.h>
#include<stdlib.h>
typedef int (*bbmain_t)(int argc , const char *argv[]);
extern uint32_t region1_start;
extern uint32_t region2_start;
extern uint32_t region3_start;
extern uint32_t region1_end;
extern uint32_t region2_end;
extern uint32_t region3_end;
extern uint32_t region1_size;
extern uint32_t region2_size;
extern uint32_t region3_size;
extern long stack_base;
extern long heap_base;
extern uint32_t stack_size;
extern uint32_t heap_size;
extern uint32_t bb_buffer_len;
extern bbmain_t bbmain;
extern void unpack_data(char *buffer);
extern char *pack_data();
extern void * exec_bbcmd(const char *argv[],uint32_t *out_len);
extern void unmapall();
