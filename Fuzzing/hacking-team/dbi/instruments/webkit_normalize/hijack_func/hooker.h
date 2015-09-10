struct hook_t *hook_hash1;

struct hook_t *postcall_hash;

#define ROUNDUP(x, y) ((((x)+((y)-1))/(y))*(y))

unsigned int base_address;

void* no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* malloc_hook(void* a) ;
void* realloc_hook(void* a, void* b) ;
void* free_hook(void* a) ;

void* new_h(void* a) ;
void* new_array_h(void* a) ;

void* render_arena_constructor_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* render_arena_allocate_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* render_object_arena_delete_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* webcore_arena_allocate_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* render_arena_free_h(void* a, void* b, void* c) ;
void* (*render_arena_free_ptr)(void* a, void* b, void* c);

void* node_normalize_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* element_normalizeAttributes_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* element_setAttribute_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* element_removeAttribute_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* attribute_d0_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* attribute_create_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* attr_d0_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* attr_create_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* text_create_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* text_d1_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* node_subtree_modified_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* element_attach_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* factory_newjsarray_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* factory_newjsarraywithelements_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* cssrule_parentstylesheet_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* cssstylesheet_insertrule_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* v8getpropertywithcallback_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* heap_fixedarraywithholes_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* heap_fixedarray1_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* heap_fixedarray2_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* CSSStyle_des_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

/* plt hooks - arm */
struct hook_t malloc_plt_hook;
struct hook_t realloc_plt_hook;
struct hook_t free_plt_hook;
struct hook_t new_array_plt_hook;
struct hook_t new_plt_hook;


/* regular thumb hooks */
struct hook_t render_arena_constructor_hook;
struct hook_t render_arena_allocate_hook;
struct hook_t render_object_arena_delete_hook;
struct hook_t webcore_arena_allocate_hook;
struct hook_t render_arena_free_hook;


struct hook_t attr_d0_hook;
struct hook_t attr_create_hook;
struct hook_t text_create_hook;
struct hook_t text_d1_hook;
struct hook_t node_normalize_hook;
struct hook_t element_normalizeAttributes_hook;
struct hook_t element_setAttribute_hook;
struct hook_t element_removeAttribute_hook;
struct hook_t attribute_d0_hook;
struct hook_t attribute_create_hook;
struct hook_t node_subtree_modified_hook;

struct hook_t element_attach_hook;
struct hook_t factory_newjsarray_hook;
struct hook_t factory_newjsarraywithelements_hook;
struct hook_t cssrule_parentstylesheet_hook;
struct hook_t cssstylesheet_insertrule_hook;
struct hook_t v8getpropertywithcallback_hook;
struct hook_t heap_fixedarray1_hook;
struct hook_t heap_fixedarray2_hook;
struct hook_t heap_fixedarraywithholes_hook;
struct hook_t CSSStyle_des_hook;

int do_malloc;

#define DES 1

#ifdef S3
#define HOOK_coverage_11 hook_no_hash(&element_attach_hook, pid, "libwebcore", "", element_attach_h, 0, 0x3576c3  );
#define HOOK_coverage_10 hook_no_hash(&element_normalizeAttributes_hook, pid, "libwebcore", "", element_normalizeAttributes_h, 0, 0x358393  );
#define HOOK_coverage_9 hook_no_hash(&node_normalize_hook, pid, "libwebcore", "", node_normalize_h, 0, 0x1165d5  );
#define HOOK_coverage_8 hook_no_hash(&render_arena_free_hook, pid, "libwebcore", "", render_arena_free_h, 0, 0x1de69b  );
#define HOOK_coverage_7 hook_no_hash(&webcore_arena_allocate_hook, pid, "libwebcore", "", webcore_arena_allocate_h, 0, 0x3bc805  );
#define HOOK_coverage_6 hook_no_hash(&render_object_arena_delete_hook, pid, "libwebcore", "", render_object_arena_delete_h, 0, 0x217bf1  );
#define HOOK_coverage_5 hook_no_hash(&render_arena_allocate_hook, pid, "libwebcore", "", render_arena_allocate_h, 0, 0x1de661 );
#define HOOK_coverage_4 hook_no_hash(&render_arena_constructor_hook, pid, "libwebcore", "", render_arena_constructor_h, 0, 0x1de62d );
#define HOOK_coverage_3 hook_no_hash(&malloc_plt_hook, pid, "libwebcore", "", malloc_hook, 0, 0x10c8ac );
#define HOOK_coverage_2 hook_no_hash(&realloc_plt_hook, pid, "libwebcore", "", realloc_hook, 0, 0x10c8f4 ); c
#define HOOK_coverage_1 hook_no_hash(&free_plt_hook, pid, "libwebcore", "", free_hook, 0, 0x10c8e8 ;)


#elif MOT

#define HOOK_coverage_29 hook_no_hash(&attr_d0_hook, pid, "libwebcore", "", attr_d0_h, 0, 0x27b1c5  );
#define HOOK_coverage_28 hook_no_hash(&attr_create_hook, pid, "libwebcore", "", attr_create_h, 0, 0x27b149  );


#define HOOK_coverage_3 hook_no_hash(&malloc_plt_hook, pid, "libwebcore", "", malloc_hook, 0, 0xafd14748 );
#define HOOK_coverage_2 
#define HOOK_coverage_1 hook_no_hash(&free_plt_hook, pid, "libwebcore", "", free_hook, 0, 0xafd14764 ;)


#elif DES


#define HOOK_coverage_22 hook_no_hash(&new_plt_hook, pid, "libwebcore", "", new_h, 0, 0x693c0  );
#define HOOK_coverage_21 hook_no_hash(&new_array_plt_hook, pid, "libwebcore", "", new_array_h, 0, 0x695ac  );

#define HOOK_coverage_20 //hook_no_hash(&CSSStyle_des_hook, pid, "libwebcore", "", CSSStyle_des_h, 0, 0x1bba61  );
#define HOOK_coverage_19 //hook_no_hash(&heap_fixedarray1_hook, pid, "libwebcore", "", heap_fixedarray1_h, 0, 0x2c35c9  );
#define HOOK_coverage_18 //hook_no_hash(&heap_fixedarray2_hook, pid, "libwebcore", "", heap_fixedarray2_h, 0, 0x2c3401  );
#define HOOK_coverage_17 //hook_no_hash(&heap_fixedarraywithholes_hook, pid, "libwebcore", "", heap_fixedarraywithholes_h, 0, 0x2c3571  );

#define HOOK_coverage_16 //hook_no_hash(&v8getpropertywithcallback_hook, pid, "libwebcore", "", v8getpropertywithcallback_h, 0, 0x2d03d9  );
#define HOOK_coverage_15 //hook_no_hash(&cssstylesheet_insertrule_hook, pid, "libwebcore", "", cssstylesheet_insertrule_h, 0, 0x1bb97d  );
#define HOOK_coverage_14 //hook_no_hash(&cssrule_parentstylesheet_hook, pid, "libwebcore", "", cssrule_parentstylesheet_h, 0, 0x277da1  );

#define HOOK_coverage_13 //hook_no_hash(&factory_newjsarraywithelements_hook, pid, "libwebcore", "", factory_newjsarraywithelements_h, 0, 0x2b7efd  );
#define HOOK_coverage_12 //hook_no_hash(&factory_newjsarray_hook, pid, "libwebcore", "", factory_newjsarray_h, 0, 0x2b8021  );

#define HOOK_coverage_11 //hook_no_hash(&element_attach_hook, pid, "libwebcore", "", element_attach_h, 0, 0x1cee69  );

#define HOOK_coverage_29 hook_no_hash(&attr_d0_hook, pid, "libwebcore", "", attr_d0_h, 0, 0x27b1c5  );
#define HOOK_coverage_28 hook_no_hash(&attr_create_hook, pid, "libwebcore", "", attr_create_h, 0, 0x27b149  );

#define HOOK_coverage_30 hook_no_hash(&text_create_hook, pid, "libwebcore", "", text_create_h, 0, 0x1e2999 ); 
#define HOOK_coverage_31 hook_no_hash(&text_d1_hook, pid, "libwebcore", "", text_create_h, 0, 0x1e241d );

#define HOOK_coverage_27 hook_no_hash(&node_subtree_modified_hook, pid, "libwebcore", "", node_subtree_modified_h, 0, 0x1d7ed5  );
#define HOOK_coverage_26 hook_no_hash(&attribute_create_hook, pid, "libwebcore", "", attribute_create_h, 0, 0x1cdc29  );
#define HOOK_coverage_25 hook_no_hash(&attribute_d0_hook, pid, "libwebcore", "", attribute_d0_h, 0, 0x1d220d  );
#define HOOK_coverage_24 hook_no_hash(&element_removeAttribute_hook, pid, "libwebcore", "", element_removeAttribute_h, 0, 0x1ce54d  );
#define HOOK_coverage_23 hook_no_hash(&element_setAttribute_hook, pid, "libwebcore", "", element_setAttribute_h, 0, 0x1cf3ad  );
#define HOOK_coverage_10 hook_no_hash(&element_normalizeAttributes_hook, pid, "libwebcore", "", element_normalizeAttributes_h, 0, 0x1cde59  );
#define HOOK_coverage_9  hook_no_hash(&node_normalize_hook, pid, "libwebcore", "", node_normalize_h, 0, 0x1d7289   ); 

#define HOOK_coverage_8 //hook_no_hash(&render_arena_free_hook, pid, "libwebcore", "", render_arena_free_h, 0, 0x100519  );
#define HOOK_coverage_7 //hook_no_hash(&webcore_arena_allocate_hook, pid, "libwebcore", "", webcore_arena_allocate_h, 0, 0x11ee01  );
#define HOOK_coverage_6 //hook_no_hash(&render_object_arena_delete_hook, pid, "libwebcore", "", render_object_arena_delete_h, 0, 0x135815  );
#define HOOK_coverage_5 //hook_no_hash(&render_arena_allocate_hook, pid, "libwebcore", "", render_arena_allocate_h, 0, 0x100531 );
#define HOOK_coverage_4 //hook_no_hash(&render_arena_constructor_hook, pid, "libwebcore", "", render_arena_constructor_h, 0, 0x100575 );

#define HOOK_coverage_3 hook_no_hash(&malloc_plt_hook, pid, "libwebcore", "", malloc_hook, 0, 0x69450 );
#define HOOK_coverage_2
#define HOOK_coverage_1 hook_no_hash(&free_plt_hook, pid, "libwebcore", "", free_hook, 0, 0x69444 );


#elif EMULATOR22

#define HOOK_coverage_20 hook_no_hash(&CSSStyle_des_hook, pid, "libwebcore", "", CSSStyle_des_h, 0, 0x1ab8f9 );

#define HOOK_coverage_19 //hook_no_hash(&heap_fixedarray1_hook, pid, "libwebcore", "", heap_fixedarray1_h, 0, 0x  );
#define HOOK_coverage_18 //hook_no_hash(&heap_fixedarray2_hook, pid, "libwebcore", "", heap_fixedarray2_h, 0, 0x  );
#define HOOK_coverage_17 //hook_no_hash(&heap_fixedarraywithholes_hook, pid, "libwebcore", "", heap_fixedarraywithholes_h, 0, 0x  );

#define HOOK_coverage_16 //hook_no_hash(&v8getpropertywithcallback_hook, pid, "libwebcore", "", v8getpropertywithcallback_h, 0, 0x  );

#define HOOK_coverage_15 hook_no_hash(&cssstylesheet_insertrule_hook, pid, "libwebcore", "", cssstylesheet_insertrule_h, 0, 0x1ab805  );
#define HOOK_coverage_14 hook_no_hash(&cssrule_parentstylesheet_hook, pid, "libwebcore", "", cssrule_parentstylesheet_h, 0, 0x2be879  );

#define HOOK_coverage_13 //hook_no_hash(&factory_newjsarraywithelements_hook, pid, "libwebcore", "", factory_newjsarraywithelements_h, 0, 0x2b7efd  );
#define HOOK_coverage_12 //hook_no_hash(&factory_newjsarray_hook, pid, "libwebcore", "", factory_newjsarray_h, 0, 0x2b8021  );

#define HOOK_coverage_11 //hook_no_hash(&element_attach_hook, pid, "libwebcore", "", element_attach_h, 0, 0x1cee69  );
#define HOOK_coverage_10 //hook_no_hash(&element_normalizeAttributes_hook, pid, "libwebcore", "", element_normalizeAttributes_h, 0, 0x1cde59  );
#define HOOK_coverage_9 //hook_no_hash(&node_normalize_hook, pid, "libwebcore", "", node_normalize_h, 0, 0x1d7288  );
#define HOOK_coverage_8 //hook_no_hash(&render_arena_free_hook, pid, "libwebcore", "", render_arena_free_h, 0, 0x100519  );
#define HOOK_coverage_7 //hook_no_hash(&webcore_arena_allocate_hook, pid, "libwebcore", "", webcore_arena_allocate_h, 0, 0x11ee01  );
#define HOOK_coverage_6 //hook_no_hash(&render_object_arena_delete_hook, pid, "libwebcore", "", render_object_arena_delete_h, 0, 0x135815  );
#define HOOK_coverage_5 //hook_no_hash(&render_arena_allocate_hook, pid, "libwebcore", "", render_arena_allocate_h, 0, 0x100531 );
#define HOOK_coverage_4 //hook_no_hash(&render_arena_constructor_hook, pid, "libwebcore", "", render_arena_constructor_h, 0, 0x100575 );
#define HOOK_coverage_3 //hook_no_hash(&malloc_plt_hook, pid, "libwebcore", "", malloc_hook, 0, 0xb7bc );
#define HOOK_coverage_2
#define HOOK_coverage_1 //hook_no_hash(&free_plt_hook, pid, "libwebcore", "", free_hook, 0, 0xb294 );


#endif

