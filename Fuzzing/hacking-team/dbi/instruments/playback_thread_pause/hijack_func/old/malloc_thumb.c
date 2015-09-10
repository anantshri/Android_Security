#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"
#include "malloc.h"

// extern __mallocfunc void*  malloc(size_t);


void* (*orig_malloc)(size_t size);

/*
void my_cb_malloc(...) // FIXME
{
	struct special_malloc_t *d = (struct special_malloc_t*) hook_malloc.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

void* my_malloc(size_t size) 
{
	orig_malloc = (void*) hook_malloc.orig;

	log("%s enter\n", __func__)
	log("orig_malloc = %x\n", orig_malloc)

	struct special_malloc_t *d = (struct special_malloc_t*)hook_malloc.data;
	//d->orig_cb = X; // FIXME
	//d->my_cb = my_cb_malloc; // FIXME
	//d->pContext = pContext;

	hook_precall(&hook_malloc);
	void* res = orig_malloc(size); 
	hook_postcall(&hook_malloc);

	log("%s result = %x\n", __func__, res)
	return res;
}
