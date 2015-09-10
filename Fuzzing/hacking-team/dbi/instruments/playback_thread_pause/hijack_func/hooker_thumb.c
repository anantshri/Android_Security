#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"
#include "hooker.h"

void* (*coverage_ptr)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r);

void*  coverage_0(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r) {
	void *result;
	coverage_ptr = (void *) hook_array[0].orig;

	log("[*]\tHit func 0x%x\n", coverage_ptr)
	hook_precall(&hook_array[0]);
	result = coverage_ptr( a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r);
	hook_postcall(&hook_array[0]);
	return result;
}

