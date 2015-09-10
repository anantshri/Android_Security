#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"
#include "blow.h"
#include "uthash.h"


static int ip_register_offset = 0xd;



void* (*no_proto_ptr)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r);

void*  no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r) {
  
  void *result;
  int ip_var;
  struct hook_t *hook_no_proto = NULL;


  asm("mov %[ip], ip" : [ip] "=r" (ip_var) );
  
  ip_var -= ip_register_offset;

  log("[*]\tHit $ip: 0x%x\n", ip_var)
    
    log("[*]\t\tSearching hash table 0x%xfor 0x%x\n", hook_hash1, ip_var)
  //find struct hook_t 
  HASH_FIND_INT(hook_hash1, &ip_var, hook_no_proto);

  if( hook_no_proto != NULL)
    log("[*]\t\tFound struct 0x%x\n", hook_no_proto)
  else
    log("[*]\t\tstruct not found\n")
      


  no_proto_ptr = (void *) hook_no_proto->orig;

  hook_precall(hook_no_proto);

  result = no_proto_ptr( a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r);
 
  hook_postcall(hook_no_proto);

  log("[*]\t\tReturning: 0x%x\n", result);

  return result;
}


int getLr() {
  asm volatile
    (
     "mov r0, lr  \n\t"
     );

}

