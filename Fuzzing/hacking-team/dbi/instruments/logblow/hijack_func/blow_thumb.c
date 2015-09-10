#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"
#include "blow.h"

//int blow(int a)

int (*orig_blow)();

/*
void my_cb_blow(...) // FIXME
{
	struct special_blow_t *d = (struct special_blow_t*) hook_blow.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

int my_blow(int a ) // FIXME
{
  int res;
  int i;

  orig_blow = (void*) hook_blow.orig;

  log("%s enter\n", __func__)

    //  log("orig_blow = %x\n", orig_blow)

    //struct special_blow_t *d = (struct special_blow_t*)hook_blow.data;

    hook_precall(&hook_blow);

  log("calling hooked function\n")
    log("address: %x - param: %d", orig_blow, a )
	  
    log("\n");

  /* for(i = 0 ; i < 0x42; i++) { */
  /*   log("%0.2x ",   *((char *) (orig_blow + i -1)) ) */
  /*     if( i!=0 && (i % 4) == 1) */
  /* 	log("\n") */
  /* 	  } */

  log("\n");

    res = orig_blow(a+3);

  log("hooked function finished\n")

    
    hook_postcall(&hook_blow);

  log("%s result = %x\n", __func__, res)
    return res;
}


/* int my_blow( ) // FIXME */
/* { */
/* 	orig_blow = (void*) hook_blow.orig; */
/* 	log("%s enter\n", __func__) */
/* 	log("orig_blow = %x\n", orig_blow) */

/* 	hook_precall(&hook_blow); */
/* 	int res = orig_blow(); */
/* 	hook_postcall(&hook_blow); */

/* 	log("%s result = %x\n", __func__, res) */
/* 	return res; */
/* } */

