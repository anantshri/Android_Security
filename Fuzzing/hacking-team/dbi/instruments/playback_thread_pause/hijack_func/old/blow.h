#define HOOKS_NUMBER 27942

struct hook_t hook_array[HOOKS_NUMBER];


char*  my_strcpy(char* dest, char* src);
struct hook_t hook_strcpy;
#define HOOK_strcpy hook(&hook_strcpy, pid, "libtest_lib", "", my_strcpy, 0, 0xf79 );


/* no proto */
void*  no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r) ;
struct hook_t hook_no_proto;
#define HOOK_no_proto hook(&hook_no_proto, pid, "libtest_lib", "", no_proto, 0, 0xf95 );

// ya_strcpy
//#define HOOK_no_proto hook(&hook_no_proto, pid, "libtest_lib", "", no_proto, 0, 0x100d );


// 2
void*  no_proto2(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h) ;
struct hook_t hook_no_proto2;
#define HOOK_no_proto2 hook(&hook_no_proto2, pid, "libtest_lib", "", no_proto2, 0, 0xf8d );
