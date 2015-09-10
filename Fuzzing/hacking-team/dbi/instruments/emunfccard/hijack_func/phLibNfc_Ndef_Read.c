#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "phNfcStatus.h"
#include "phNfcTypes.h"
#include "phLibNfc.h"
#include "phFriNfc_NdefMap.h"
#include "libt.h"
#include "util.h"
#include "phLibNfc_Ndef_Read.h"

#define FAKE_CARD "/data/local/tmp/FAKE_CARD"

NFCSTATUS (*orig_phLibNfc_Ndef_Read)( phLibNfc_Handle                   hRemoteDevice,
                            phNfc_sData_t                      *psRd,
                            phLibNfc_Ndef_EOffset_t             Offset,
                            pphLibNfc_RspCb_t                   pNdefRead_RspCb,
                            void*                               pContext);

void my_cb_phLibNfc_Ndef_Read(void* pContext,NFCSTATUS  Status);

NFCSTATUS my_phLibNfc_Ndef_Read( phLibNfc_Handle                   hRemoteDevice,
                            phNfc_sData_t                      *psRd,
                            phLibNfc_Ndef_EOffset_t             Offset,
                            pphLibNfc_RspCb_t                   pNdefRead_RspCb,
                            void*                               pContext)
{
	orig_phLibNfc_Ndef_Read = (void*) hook_phLibNfc_Ndef_Read.orig;
	log("%s enter\n", __func__)
	log("orig_phLibNfc_Ndef_Read = %x\n", orig_phLibNfc_Ndef_Read)

	struct special_phLibNfc_Ndef_Read_t *d = (struct special_phLibNfc_Ndef_Read_t*)hook_phLibNfc_Ndef_Read.data;
	d->orig_cb = pNdefRead_RspCb; // FIXME
	d->my_cb = my_cb_phLibNfc_Ndef_Read; // FIXME
	d->psRd = psRd;
	d->pContext = pContext;
	d->Offset = Offset;

	if (fake_card_state == 3) {
		log("fake card state = %x\n", fake_card_state)
		my_cb_phLibNfc_Ndef_Read(pContext, NFCSTATUS_SUCCESS);
		return 0xd;
	}

	hook_precall(&hook_phLibNfc_Ndef_Read);
	NFCSTATUS res = orig_phLibNfc_Ndef_Read(hRemoteDevice, psRd, Offset, my_cb_phLibNfc_Ndef_Read, pContext); // FIXME
	hook_postcall(&hook_phLibNfc_Ndef_Read);

	log("%s result = %x\n", __func__, res)
	return res;
}

extern int fake_card_len;
extern unsigned char *fake_card;

void my_cb_phLibNfc_Ndef_Read(void* pContext,NFCSTATUS  Status)
{
	char buffer[1024];
	struct special_phLibNfc_Ndef_Read_t *d = (struct special_phLibNfc_Ndef_Read_t*) hook_phLibNfc_Ndef_Read.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	if (fake_card_state == 0) {
		log("psRd->length = %d\n", d->psRd->length)
		int i;
		for (i = 0; i < d->psRd->length; i++) {
			if (i%16 == 0) log("\n")
			log("%0.2x", d->psRd->buffer[i]);
		}
		log("\n")
	}
	
	if (fake_card_len > 0 && fake_card != NULL) {
		free(d->psRd->buffer);
		d->psRd->buffer = fake_card;
		d->psRd->length = fake_card_len;
		fake_card_state = 0;
		log("%s: filled fake data\n", __func__)
	}
	/*
	if (access(FAKE_CARD, F_OK) == 0) {
		int fp = open(FAKE_CARD, O_RDONLY);
		int s = 0;
		if (fp >= 0) {
			s = read(fp, buffer, sizeof(buffer));
			log("%s: read %d bytes\n", __func__, s)
			close(fp);
		}
		if (s > 0) {
			free(d->psRd->buffer);
			d->psRd->buffer = malloc(s);
			memcpy(d->psRd->buffer, buffer, s);
			d->psRd->length = s;
			log("%s: filled fake data\n", __func__)
			unlink(FAKE_CARD);
		}
		fake_card_state = 0;
	}
	*/

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
