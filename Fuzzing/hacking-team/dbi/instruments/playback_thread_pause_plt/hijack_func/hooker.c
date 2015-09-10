#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"

#include "hooker.h"


//void* (*new_h_ptr)(void* a);

/* void* (*pause_h_ptr)(void* a, void* b, void* c, void* d, void* e); */

/* void*  pause_h(void *a, void *b, void* c, void* d, void* e) { */
  
/*   void *result; */
  
/*   log("enter pause\n") */

/*   pause_h_ptr = (void *) pause_plt_hook.orig; */

/*   log("precall\n"); */
/*   hook_precall(&pause_plt_hook); */

/*   log("call %p\n", pause_h_ptr); */
/*   result = pause_h_ptr(a, b, c, d, e); */

/*   log("postcall\n"); */
/*   hook_postcall(&pause_plt_hook); */
  
/*   log("pause hit\n") */

/*   return result; */
/* } */


/* void*  new_array_h(void* a) { */
  
/*   void *result; */

/*   new_array_h_ptr = (void *) new_array_plt_hook.orig; */

/*   hook_precall(&new_array_plt_hook); */

/*   result = new_array_h_ptr(  a); */

/*   hook_postcall(&new_array_plt_hook); */
  
/*   if( do_malloc) */
/*     log("new array: %d bytes @ %p\n", a, result) */

/*   return result; */
/* } */

