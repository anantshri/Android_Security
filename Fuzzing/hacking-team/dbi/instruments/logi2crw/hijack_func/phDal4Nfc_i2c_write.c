#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"
#include "phDal4Nfc_i2c_write.h"

int (*orig_phDal4Nfc_i2c_write)(char * pBuffer, int nNbBytesToWrite);

/*
void my_cb_phDal4Nfc_i2c_write(...) // FIXME
{
	struct special_phDal4Nfc_i2c_write_t *d = (struct special_phDal4Nfc_i2c_write_t*) hook_phDal4Nfc_i2c_write.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

int my_phDal4Nfc_i2c_write(char * pBuffer, int nNbBytesToWrite)
{
	orig_phDal4Nfc_i2c_write = (void*) hook_phDal4Nfc_i2c_write.orig;
//	log("%s enter\n", __func__)
//	log("orig_phDal4Nfc_i2c_write = %x\n", orig_phDal4Nfc_i2c_write)
	int i;
	struct special_phDal4Nfc_i2c_write_t *d = (struct special_phDal4Nfc_i2c_write_t*)hook_phDal4Nfc_i2c_write.data;
	
	log("--write %d bytes --\n", nNbBytesToWrite)
	for (i = 0; i < nNbBytesToWrite; i++) {
		log("%0.2x", pBuffer[i])
	}
	log("\n")
	
	hook_precall(&hook_phDal4Nfc_i2c_write);
	int res = orig_phDal4Nfc_i2c_write(pBuffer, nNbBytesToWrite);
	hook_postcall(&hook_phDal4Nfc_i2c_write);

	log("%s result = %x\n", __func__, res)
	return res;
}
