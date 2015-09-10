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

#define DEBUG 0


void* (*no_proto_ptr)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p);

void*  no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p) {
  
  void *result;
  int ip_var;
  struct hook_t *hook_no_proto = NULL;


  asm("mov %[ip], ip" : [ip] "=r" (ip_var) );
  
 
  ip_var -= ip_register_offset;

  log("%x\n", ip_var - base_address)


  //find struct hook_t 
  //HASH_FIND_INT(hook_hash1, &ip_var, hook_no_proto);




    int jj = 0;
  int found = 0 ;
  for( ; jj < 6258; jj++) {
    //log("[*] %x - %x\n", dispatcher_array[jj]->address, ip_var)

      if( ip_var == dispatcher_array[jj]->address ) {
	log("[*]\t Found dispatcher!\n")
	hook_no_proto = (void *) dispatcher_array[jj]->hook;
	found = 1;
	break;
      }
  }

  if( ! found ) 
    log("[*] Hook not found, killme\n")

  no_proto_ptr = (void *) hook_no_proto->orig;

  hook_precall(hook_no_proto);

  result =  no_proto_ptr( a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p);


 
  //hook_postcall(hook_no_proto);

  //log("[*]\t\tReturning: 0x%x\n", result);

  return result;
}


int getLr() {
  asm volatile
    (
     "mov r0, lr  \n\t"
     );

}

