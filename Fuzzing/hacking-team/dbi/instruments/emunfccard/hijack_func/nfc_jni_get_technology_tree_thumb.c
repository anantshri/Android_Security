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
#include "nfc_jni_get_technology_tree.h"

//#include "com_android_nfc.h"
//#include "com_android_nfc_list.h"

NFCSTATUS
(*orig_nfc_jni_get_technology_tree)(void* e, phLibNfc_RemoteDevList_t* devList,
        uint8_t count, void* techList, void* handleList,
		        void* libnfcTypeList); //FIXME

/*
void my_cb_nfc_jni_get_technology_tree(...) // FIXME
{
	struct special_nfc_jni_get_technology_tree_t *d = (struct special_nfc_jni_get_technology_tree_t*) hook_nfc_jni_get_technology_tree.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

NFCSTATUS my_nfc_jni_get_technology_tree(void* e, phLibNfc_RemoteDevList_t* devList,
        uint8_t count, void* techList, void* handleList,
        void* libnfcTypeList) // FIXME
{
	orig_nfc_jni_get_technology_tree = (void*) hook_nfc_jni_get_technology_tree.orig;
	log("%s enter\n", __func__)
	log("orig_nfc_jni_get_technology_tree = %x\n", orig_nfc_jni_get_technology_tree)

	struct special_nfc_jni_get_technology_tree_t *d = (struct special_nfc_jni_get_technology_tree_t*)hook_nfc_jni_get_technology_tree.data;
	//d->orig_cb = X; // FIXME
	//d->my_cb = my_cb_nfc_jni_get_technology_tree; // FIXME
	//d->pContext = pContext;

	log(" %x\n", e)
	log(" %x\n", devList)
	log(" %x\n", count)
	log(" %x\n", techList)
	log(" %x\n", handleList)
	log(" %x\n", libnfcTypeList)
	hook_precall(&hook_nfc_jni_get_technology_tree);
	NFCSTATUS res = orig_nfc_jni_get_technology_tree(e,devList,count,techList,handleList,libnfcTypeList); // FIXME
	hook_postcall(&hook_nfc_jni_get_technology_tree);
	
	log("%s result = %x\n", __func__, res)
	return res;
}
