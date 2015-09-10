#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "phNfcStatus.h"
#include "phNfcTypes.h"
#include "phLibNfc.h"
#include "phFriNfc_NdefMap.h"
#include "libt.h"
#include "util.h"
#include "-h.h"

NFCSTATUS
(*orig_-h)(...); //FIXME

/*
void my_cb_-h(...) // FIXME
{
	struct special_-h_t *d = (struct special_-h_t*) hook_-h.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

NFCSTATUS my_-h() // FIXME
{
	orig_-h = (void*) hook_-h.orig;
	log("%s enter\n", __func__)
	log("orig_-h = %x\n", orig_-h)

	struct special_-h_t *d = (struct special_-h_t*)hook_-h.data;
	d->orig_cb = X; // FIXME
	d->my_cb = my_cb_-h; // FIXME
	//d->pContext = pContext;

	hook_precall(&hook_-h);
	NFCSTATUS res = orig_-h(...); // FIXME
	hook_postcall(&hook_-h);

	log("%s result = %x\n", __func__, res)
	return res;
}
