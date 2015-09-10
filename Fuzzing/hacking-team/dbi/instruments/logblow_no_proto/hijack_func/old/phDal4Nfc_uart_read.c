#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"
#include "phDal4Nfc_uart_read.h"

int (*orig_phDal4Nfc_uart_read)(unsigned char *pBuffer, int nNbBytesToRead);

/*
void my_cb_phDal4Nfc_uart_read(...) // FIXME
{
	struct special_phDal4Nfc_uart_read_t *d = (struct special_phDal4Nfc_uart_read_t*) hook_phDal4Nfc_uart_read.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

int my_phDal4Nfc_uart_read(unsigned char *pBuffer, int nNbBytesToRead)
{
	orig_phDal4Nfc_uart_read = (void*) hook_phDal4Nfc_uart_read.orig;

	//log("%s enter\n", __func__)
	//log("orig_phDal4Nfc_uart_read = %x\n", orig_phDal4Nfc_uart_read)
	
	int i;
	struct special_phDal4Nfc_uart_read_t *d = (struct special_phDal4Nfc_uart_read_t*)hook_phDal4Nfc_uart_read.data;

	hook_precall(&hook_phDal4Nfc_uart_read);

	int res = orig_phDal4Nfc_uart_read(pBuffer, nNbBytesToRead);

	hook_postcall(&hook_phDal4Nfc_uart_read);

	log("--read %d bytes --\n",nNbBytesToRead )
	for (i = 0; i < nNbBytesToRead; i++) {
		log("%0.2x", pBuffer[i])
	}
	log("\n")

	log("%s result = %x\n", __func__, res)
	return res;
}
