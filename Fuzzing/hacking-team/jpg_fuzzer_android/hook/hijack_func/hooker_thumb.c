#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"

#include "hooker.h"
#include "uthash.h"


static int ip_register_offset = 0xf;


void* (*no_proto_thumb_ptr)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w );

void*  no_proto_thumb(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) {
  
  void *result;
  int ip_var;
  int locked_here = 0;
  struct hook_t *hook_no_proto = NULL;

  asm("mov %[ip], ip" : [ip] "=r" (ip_var) ); // original address in r12 - 0xf
  ip_var -= ip_register_offset;
  
  if(tid_locker != gettid()) { // Check if this thread locked already the mutex
    if(LOCK_DEBUG) 
      log("Getting LOCK by %d\n",gettid())

    pthread_mutex_lock(&M); // If not lock the mutex

    if(LOCK_DEBUG)
      log("LOCK got by %d\n",gettid())

    locked_here = 1;
    tid_locker = gettid();
  }


  HASH_FIND_INT(hook_hash1, &ip_var, hook_no_proto);
  no_proto_thumb_ptr = (void *) hook_no_proto->orig;

  if(LOCK_DEBUG)
    log("executing %x from tid %d\n", no_proto_thumb_ptr, gettid());

  // Removing hook ...
  hook_precall(hook_no_proto);

  struct hook_t *hooked = malloc(sizeof(struct hook_t));
  hooked = memcpy(hooked, hook_no_proto, sizeof(struct hook_t));
  HASH_ADD_INT(hooked_list_hash, orig, hooked);

  result = no_proto_thumb_ptr(  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  w  );
  

  log("%x thumb %x %s\n", ip_var - hook_no_proto->lib_base_addr, hook_no_proto->lib_base_addr, hook_no_proto->libname)

  // If this thread got the mutex here unlock it
  if(tid_locker == gettid() && locked_here) {
    locked_here = 0;
    tid_locker = 0;
    pthread_mutex_unlock(&M);

    if(LOCK_DEBUG)
      log("LOCK released by %d\n",gettid())
  }


  return result;

}
