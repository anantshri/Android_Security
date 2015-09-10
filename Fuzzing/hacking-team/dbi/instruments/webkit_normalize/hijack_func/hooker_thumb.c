#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"

#include "hooker.h"
#include "uthash.h"



static int ip_register_offset = 0xd;


void* (*no_proto_ptr)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w );

void* (*render_arena_free_ptr) (void* a, void* b, void* c);



void* CSSStyle_des_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w)  {
 void *result;
  struct hook_t *hook_no_proto = NULL;

  no_proto_ptr = (void *) CSSStyle_des_hook.orig;

  hook_precall(&CSSStyle_des_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&CSSStyle_des_hook);

  log("CSSStyle::~CSSStyle this %p, parentStyle r0 %p parentStyle r3 %p\n", a, *(int*)a, *(*((int**)a)+8)  )

  return result;
}


void* heap_fixedarray1_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {
 void *result;
  struct hook_t *hook_no_proto = NULL;

  no_proto_ptr = (void *) heap_fixedarray1_hook.orig;

  hook_precall(&heap_fixedarray1_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&heap_fixedarray1_hook);
 
  //if( do_malloc)
    log("Heap::AllocateFixedArray1 this %p int %p result %p\n", a, b, result )

  return result;
}

void* heap_fixedarray2_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {
 void *result;
  struct hook_t *hook_no_proto = NULL;

  no_proto_ptr = (void *) heap_fixedarray2_hook.orig;

  hook_precall(&heap_fixedarray2_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&heap_fixedarray2_hook);

  //if( do_malloc)
    log("Heap::AllocateFixedArray2 this %p int %p flag %p result %p\n", a, b, c,result )

  return result;
}


void* heap_fixedarraywithholes_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {
 void *result;
  struct hook_t *hook_no_proto = NULL;

  no_proto_ptr = (void *) heap_fixedarraywithholes_hook.orig;

  hook_precall(&heap_fixedarraywithholes_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&heap_fixedarraywithholes_hook);

  //if( do_malloc)
    log("Heap::AllocateFixedArrayWithHoles this %p int %p result %p\n", a, b, result )

  return result;

}



void* v8getpropertywithcallback_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) { 
  void *result;
  struct hook_t *hook_no_proto = NULL;

  no_proto_ptr = (void *) v8getpropertywithcallback_hook.orig;

  hook_precall(&v8getpropertywithcallback_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&v8getpropertywithcallback_hook);

  log("v8::GetPropertyWithCallback this 0x%x ObjA 0x%x ObjB 0x%x String 0x%x ObjC 0x%x result 0x%x\n", a, b, c, d, e, result )

  return result;
}

void* cssstylesheet_insertrule_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {
  void *result;
  struct hook_t *hook_no_proto = NULL;

  no_proto_ptr = (void *) cssstylesheet_insertrule_hook.orig;

  hook_precall(&cssstylesheet_insertrule_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&cssstylesheet_insertrule_hook);

  
  log("CSSStyleSheet::insertRule obj 0x%x string 0x%x int %d int 0x%x result 0x%x\n", a, b, c, d, result )

    if( !do_malloc) { 
      log("adding malloc hook %d\n", pid_global)
    	do_malloc = 1;
      hook_no_hash(&malloc_plt_hook, pid_global, "libwebcore", "", malloc_hook, 0, 0x69450 );
    }

  return result;
}



void* cssrule_parentstylesheet_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w)  {
  void *result;
  struct hook_t *hook_no_proto = NULL;

  no_proto_ptr = (void *) cssrule_parentstylesheet_hook.orig;

  hook_precall(&cssrule_parentstylesheet_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&cssrule_parentstylesheet_hook);

  log("CSSRule::parentStyleSheet obj %p r4 %p r0 %p r3 %p\n", a, *(int*)(a+8), *(*(int**)(a+8)), (*(*(int***)(a+8))+8) )

  return result;
}

void* factory_newjsarray_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w)  {
  void *result;
  struct hook_t *hook_no_proto = NULL;

  no_proto_ptr = (void *) factory_newjsarray_hook.orig;

  hook_precall(&factory_newjsarray_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&factory_newjsarray_hook);

  log("v8::internal::Factory::NewJSArray obj 0x%x int %d flag 0x%x \n", result, b, c )

  return result;
}

void* factory_newjsarraywithelements_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w)  {
  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Element::attach obj 0x%x \n", a )

  no_proto_ptr = (void *) factory_newjsarraywithelements_hook.orig;

  hook_precall(&factory_newjsarraywithelements_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&factory_newjsarraywithelements_hook);

  log("v8::internal::Factory::NewJSArrayWithElements obj 0x%x fixed array 0x%x flag 0x%x \n", result, b, c )

  return result;
}

void* element_attach_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w)  {
  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Element::attach\n")

  no_proto_ptr = (void *) element_attach_hook.orig;

  hook_precall(&element_attach_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&element_attach_hook);



  return result;
}

void* element_removeAttribute_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Element::removeAttribute obj 0x%x String A %p int 0x%x\n", a, b, c )

  no_proto_ptr = (void *) element_removeAttribute_hook.orig;

  hook_precall(&element_removeAttribute_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&element_removeAttribute_hook);

  log("Element::removeAttribute end obj 0x%x \n", a )

  return result;


}

void* attr_create_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Attr::create A %p B %p C %p ",  a, b ,c )

  no_proto_ptr = (void *) attr_create_hook.orig;

  hook_precall(&attr_create_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  log("result %p\n", result)

  hook_postcall(&attr_create_hook);

  return result;
}

void* text_create_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Text::create\n")

  no_proto_ptr = (void *) text_create_hook.orig;

  hook_precall(&text_create_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  log("result %p\n", result)

  hook_postcall(&text_create_hook);

  return result;
}


void* text_d1_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Text::d1 A %p",  a )

  no_proto_ptr = (void *) text_d1_hook.orig;

  hook_precall(&text_d1_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&text_d1_hook);

  return result;
}




void* attr_d0_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;
  
  log("Attr::d0 obj %p \n", a )

  no_proto_ptr = (void *) attr_d0_hook.orig;

  hook_precall(&attr_d0_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&attr_d0_hook);

  return result;
}


void* attribute_create_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Attribute::create A %p B %p C %p ",  a, b ,c )

  no_proto_ptr = (void *) attribute_create_hook.orig;

  hook_precall(&attribute_create_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  log("result %p\n", result)

  hook_postcall(&attribute_create_hook);

  return result;
}


void* attribute_d0_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;
  
  log("Attribute::d0 obj %p \n", a )

  no_proto_ptr = (void *) attribute_d0_hook.orig;

  hook_precall(&attribute_d0_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&attribute_d0_hook);

  return result;
}


void* element_setAttribute_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Element::setAttribute obj 0x%x String A %p String B %p int 0x%x \n", a, b, c, d )

  no_proto_ptr = (void *) element_setAttribute_hook.orig;

  hook_precall(&element_setAttribute_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&element_setAttribute_hook);

  return result;

}

void* node_subtree_modified_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Node::dispatchSubtreeModifiedEvent start obj %p\n", a )

  no_proto_ptr = (void *) node_subtree_modified_hook.orig;

  hook_precall(&node_subtree_modified_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&node_subtree_modified_hook);

  log("Node::dispatchSubtreeModifiedEvent end obj %p\n", a  )
  return result;

}


void* element_normalizeAttributes_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Element::normalizeAttributes start obj 0x%x \n", a )

  no_proto_ptr = (void *) element_normalizeAttributes_hook.orig;

  hook_precall(&element_normalizeAttributes_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&element_normalizeAttributes_hook);

  log("Element::normalizeAttributes end obj %p\n", a)

  return result;


}



void* node_normalize_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  log("Node::normalize start obj 0x%x 0x%x\n", a, b )
    
  no_proto_ptr = (void *) node_normalize_hook.orig;

  hook_precall(&node_normalize_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&node_normalize_hook);

  log("Node::normalize end 0x%x 0x%x\n", a, b )

  return result;

}



void* render_arena_free_h(void* a, void* b, void* c) {

  void *result;
  struct hook_t *hook_no_proto = NULL;

  render_arena_free_ptr  = (void *) render_arena_free_hook.orig;

  hook_precall(&render_arena_free_hook);

  render_arena_free_ptr(  a,  b,  c );

  hook_postcall(&render_arena_free_hook);

  //if( do_malloc) 
    log("RenderArena::free  ptr of size %d @ 0x%x : 0x%x\n", b, c, *(int*)c )

  return NULL;
}


void* webcore_arena_allocate_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  int ip_var;
  int lr;
  struct hook_t *hook_no_proto = NULL, *hook_to_add = NULL;


  asm("mov %[zp], ip" : [zp] "=r" (ip_var) );
  
  ip_var -= ip_register_offset;

  no_proto_ptr = (void *) webcore_arena_allocate_hook.orig;

  hook_precall(&webcore_arena_allocate_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&webcore_arena_allocate_hook);

  //if( do_malloc) 
    log("WebCore::ArenaAllocate @ 0x%x pool 0x%x bytes %d\n", result, a, b )

  return result;

}


void* render_arena_allocate_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  int ip_var;
  int lr;
  struct hook_t *hook_no_proto = NULL, *hook_to_add = NULL;

  size_t index, size;
  
  size = ROUNDUP(( size_t) b, sizeof(void*));
  index = size >> ( (sizeof(void*) == 8) ? 3 : 2 );
  
  // The recycler array is sparse with the indices being multiples of the
  // rounding size, sizeof(void*), i.e., 0, 4, 8, 12, 16, 20, ... on 32-bit.
  //log("number of recyclers %d\n", 1024 >> ( (sizeof(void*) == 8) ? 3 : 2) )

  asm("mov %[zp], ip" : [zp] "=r" (ip_var) );
  
  ip_var -= ip_register_offset;

  no_proto_ptr = (void *) render_arena_allocate_hook.orig;

  hook_precall(&render_arena_allocate_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&render_arena_allocate_hook);

  //if( do_malloc) 
    log("RenderArena::allocate arena 0x%x %8d bytes @ 0x%x rounded: %d index %d : 0x%x\n", a, b, result, size, index, *(int*)result )

  return result;

}




void* render_arena_constructor_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  int ip_var;
  int lr;
  struct hook_t *hook_no_proto = NULL, *hook_to_add = NULL;


  asm("mov %[zp], ip" : [zp] "=r" (ip_var) );
  
  ip_var -= ip_register_offset;

  //log("%x\n", ip_var - base_address)

  
  no_proto_ptr = (void *) render_arena_constructor_hook.orig;

  hook_precall(&render_arena_constructor_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&render_arena_constructor_hook);

  //if( do_malloc) 
    log("RenderArena::RenderArena %d bytes @ 0x%x\n", b, result)

  return result;

}

void* render_object_arena_delete_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {

  void *result;
  int ip_var;
  int lr;
  struct hook_t *hook_no_proto = NULL, *hook_to_add = NULL;


  asm("mov %[zp], ip" : [zp] "=r" (ip_var) );
  
  ip_var -= ip_register_offset;

  
  no_proto_ptr = (void *) render_object_arena_delete_hook.orig;

  hook_precall(&render_object_arena_delete_hook);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  hook_postcall(&render_object_arena_delete_hook);

  //if( do_malloc) 
    log("RenderObject::destroy  0x%x\n", a)

  return result;

}



void*  no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {
  
  void *result;
  int ip_var;
  struct hook_t *hook_no_proto = NULL, *hook_to_add = NULL;


  asm("mov %[ip], ip" : [ip] "=r" (ip_var) );
  
  ip_var -= ip_register_offset;

  log("%x\n", ip_var - base_address)


  //find struct hook_t 
  HASH_FIND(hh, hook_hash1, &ip_var, sizeof(int), hook_no_proto);

 
  if( hook_no_proto == NULL)
    log("Damned\n")


  // for postcall reset
  HASH_FIND(hh_postcall, postcall_hash, &ip_var, sizeof(int), hook_to_add);

  if( hook_to_add == NULL ) {
    HASH_ADD(hh_postcall, postcall_hash, orig, sizeof(int), hook_no_proto);
  } 

  no_proto_ptr = (void *) hook_no_proto->orig;

  hook_precall(hook_no_proto);

  result = no_proto_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );

  //hook_postcall(hook_no_proto);

  return result;
}

