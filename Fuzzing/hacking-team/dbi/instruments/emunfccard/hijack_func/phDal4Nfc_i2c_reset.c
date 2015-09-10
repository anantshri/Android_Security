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
#include "phDal4Nfc_i2c_reset.h"

int (*orig_phDal4Nfc_i2c_reset)(long level); //FIXME

/*
void my_cb_phDal4Nfc_i2c_reset(...) // FIXME
{
	struct special_phDal4Nfc_i2c_reset_t *d = (struct special_phDal4Nfc_i2c_reset_t*) hook_phDal4Nfc_i2c_reset.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

int my_phDal4Nfc_i2c_reset(long level) // FIXME
{
	orig_phDal4Nfc_i2c_reset = (void*) hook_phDal4Nfc_i2c_reset.orig;
	log("%s enter\n", __func__)
	log("orig_phDal4Nfc_i2c_reset = %x\n", orig_phDal4Nfc_i2c_reset)

	struct special_phDal4Nfc_i2c_reset_t *d = (struct special_phDal4Nfc_i2c_reset_t*)hook_phDal4Nfc_i2c_reset.data;
//	d->orig_cb = X; // FIXME
//	d->my_cb = my_cb_phDal4Nfc_i2c_reset; // FIXME
	//d->pContext = pContext;

//	hook_precall(&hook_phDal4Nfc_i2c_reset);
//	NFCSTATUS res = orig_phDal4Nfc_i2c_reset(level); // FIXME
//	hook_postcall(&hook_phDal4Nfc_i2c_reset);

	log("%s result = %x <-- we set this by hand\n", __func__, 0)
	return 0; //res;
}
