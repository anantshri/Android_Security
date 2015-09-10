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

struct hook_t hook_phFriNfc_MifareStdMap_RdNdef;

struct special_phFriNfc_MifareStdMap_RdNdef_t {
	pphLibNfc_RspCb_t *orig_cb;
	pphLibNfc_RspCb_t *my_cb;
} special_phFriNfc_MifareStdMap_RdNdef;

NFCSTATUS
(*orig_phFriNfc_MifareStdMap_RdNdef)(); //FIXME

my_phFriNfc_MifareStdMap_RdNdef() // FIXME
{
	orig_phFriNfc_MifareStdMap_RdNdef = (void*) hook_phFriNfc_MifareStdMap_RdNdef.orig;
	log("%s enter\n", __func__)
	log("orig_phFriNfc_MifareStdMap_RdNdef = %x\n", orig_phFriNfc_MifareStdMap_RdNdef)

	struct special_phFriNfc_MifareStdMap_RdNdef_t *d = (struct special_phFriNfc_MifareStdMap_RdNdef_t*)hook_phFriNfc_MifareStdMap_RdNdef.data;
	d->orig_cb = 0; // FIXME
	d->my_cb = 0; // FIXME
	//d->pContext = pContext;

	hook_precall(&hook_phFriNfc_MifareStdMap_RdNdef);
	NFCSTATUS res = orig_phFriNfc_MifareStdMap_RdNdef(); // FIXME
	hook_postcall(&hook_phFriNfc_MifareStdMap_RdNdef);

	log("%s result = %x\n", __func__, res)
	return res;
}

/*
my_cb_phFriNfc_MifareStdMap_RdNdef(...)
{
	struct special_phFriNfc_MifareStdMap_RdNdef *d = (struct special_phFriNfc_MifareStdMap_RdNdef*) hook_phFriNfc_MifareStdMap_RdNdef.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/
