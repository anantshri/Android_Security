#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"
#include "blow.h"


char* (*strcpy_ptr)(char* dst, char* src);

char*  my_strcpy(char* dest, char* src) {

  log("[*] Hit strcpy\n")

  printf("WTF\n");
 
  strcpy_ptr = (void *) hook_strcpy.orig;

  hook_precall(&hook_strcpy);
  return strcpy_ptr(dest, src);
  
  
  //hook_postcall(&hook_strcpy);


}



void* (*no_proto_ptr)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r);

void*  no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r) {
  
  void *result;


  no_proto_ptr = (void *) hook_no_proto.orig;

  log("[*]\tHit no_proto 0x%x\n", no_proto_ptr)
    //log("[*]\t%s\n", e)

  hook_precall(&hook_no_proto);

  result = no_proto_ptr( a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r);
 
  hook_postcall(&hook_no_proto);

  log("[*]\t\tReturning: 0x%x\n", result);

  return result;
}

void* (*no_proto_ptr2)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h);
void*  no_proto2(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h) {
  
  void *result;


  no_proto_ptr2 = (void *) hook_no_proto2.orig;

  log("[*] Hit no_proto2 0x%x\n", no_proto_ptr2)

  hook_precall(&hook_no_proto2);

  result = no_proto_ptr2( a,  b,  c,  d,  e,  f,  g,  h);
 
  hook_postcall(&hook_no_proto2);

  return result;
}
