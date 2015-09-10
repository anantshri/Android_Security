#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"
#include "BMalloc.h"

void* (*orig_BMalloc)(size_t size);

/*
void my_cb_BMalloc(...) // FIXME
{
	struct special_BMalloc_t *d = (struct special_BMalloc_t*) hook_BMalloc.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

void* my_BMalloc(size_t size)
{
  
  
  log("Hook entry point")
  
  orig_BMalloc = (void*) hook_BMalloc.orig;
	
  log("orig_BMalloc = \n")
	  

  hook_precall(&hook_BMalloc);

  void* res = orig_BMalloc(size);

  hook_postcall(&hook_BMalloc);

  log("--allocated bytes --\n")
	
  return res;

}
