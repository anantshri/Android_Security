struct hook_t *hook_hash1;

void*  no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r) ;

#define HOOK_no_proto_0 hook(hook_hash1, pid, "libtest_lib", "", no_proto, 0, 0xfad );
#define HOOK_no_proto_1 hook(hook_hash1, pid, "libtest_lib", "", no_proto, 0, 0xf95 );

