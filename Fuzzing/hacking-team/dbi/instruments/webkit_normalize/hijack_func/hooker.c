#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"

#include "hooker.h"


void* (*malloc_hook_ptr)(void* a);
void* (*realloc_hook_ptr)(void* a, void* b);
void* (*free_hook_ptr)(void* a);
void* (*new_array_h_ptr)(void* a);
void* (*new_h_ptr)(void* a);

void*  new_array_h(void* a) {
  
  void *result;

  new_array_h_ptr = (void *) new_array_plt_hook.orig;

  hook_precall(&new_array_plt_hook);

  result = new_array_h_ptr(  a);

  hook_postcall(&new_array_plt_hook);
  
  //if( do_malloc)
    log("new array: %d bytes @ %p\n", a, result)

  return result;
}


void*  new_h(void* a) {
  
  void *result;

  new_h_ptr = (void *) new_plt_hook.orig;
  
  hook_precall(&new_plt_hook);
 
  result = new_h_ptr(  a);

  hook_postcall(&new_plt_hook);
  
  //if( do_malloc)
    log("new: %d bytes @ %p\n", a, result)

  return result;
}


void*  malloc_hook(void* a) {
  
  void *result;

  malloc_hook_ptr = (void *) malloc_plt_hook.orig;
  hook_precall(&malloc_plt_hook);
  result = malloc_hook_ptr(  a);

  hook_postcall(&malloc_plt_hook);
  
  //if( do_malloc)
    log("malloc: %d bytes @ %p\n", a, result)

  return result;
}




void*  realloc_hook(void* a, void* b) {
  
  void *result;

  realloc_hook_ptr = (void *) realloc_plt_hook.orig;

  hook_precall(&realloc_plt_hook);

  result = realloc_hook_ptr(  a,  b );

  hook_postcall(&realloc_plt_hook);

  log("realloc: %d bytes for ptr %p @ %p\n", b, a, result)

  return result;
}


void*  free_hook(void* a) {
  
  void *result;

  free_hook_ptr = (void *) free_plt_hook.orig;
  hook_precall(&free_plt_hook);
  result = free_hook_ptr(  a );
  hook_postcall(&free_plt_hook);

  if( a != 0)
    log("free: @ %p\n", a)

  return result;
}

