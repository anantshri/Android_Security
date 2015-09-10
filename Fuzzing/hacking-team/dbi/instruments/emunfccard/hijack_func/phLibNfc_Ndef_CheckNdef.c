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

#include "hijacks.h"


NFCSTATUS
(*orig_phLibNfc_Ndef_CheckNdef)(phLibNfc_Handle       hRemoteDevice,
                        pphLibNfc_ChkNdefRspCb_t        pCheckNdef_RspCb,
                        void*                           pContext); //FIXME

/*
void my_cb_phLibNfc_Ndef_CheckNdef(...) // FIXME
{
	struct special_phLibNfc_Ndef_CheckNdef_t *d = (struct special_phLibNfc_Ndef_CheckNdef_t*) hook_phLibNfc_Ndef_CheckNdef.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

NFCSTATUS my_phLibNfc_Ndef_CheckNdef(phLibNfc_Handle       hRemoteDevice,
                        pphLibNfc_ChkNdefRspCb_t        pCheckNdef_RspCb,
                        void*                           pContext) 
{
	orig_phLibNfc_Ndef_CheckNdef = (void*) hook_phLibNfc_Ndef_CheckNdef.orig;
	log("%s enter\n", __func__)
	log("orig_phLibNfc_Ndef_CheckNdef = %x\n", orig_phLibNfc_Ndef_CheckNdef)

	struct special_phLibNfc_Ndef_CheckNdef_t *d = (struct special_phLibNfc_Ndef_CheckNdef_t*)hook_phLibNfc_Ndef_CheckNdef.data;
	d->orig_cb = pCheckNdef_RspCb; // FIXME
	d->my_cb = NULL; //my_cb_phLibNfc_Ndef_CheckNdef; // FIXME
	d->pContext = pContext;

	if (fake_card_state == 2) {
		log("fake_card_state 2->3\n")
		fake_card_state = 3;
		//d->orig_cb(pContext, NFCSTATUS_SUCCESS);
		log("%s wrapper = %x\n", __func__, wrapper.msg.eMsgType)
		// trigger callback in msgrcv(..)
		wrapper.msg.eMsgType = 0xffffff02;
		ptr_phDal4Nfc_msgsnd = msgsend;				
		if (ptr_phDal4Nfc_msgsnd != NULL) {
			ptr_phDal4Nfc_msgsnd(global_msqid, (struct msgbuf *)&wrapper, sizeof(phLibNfc_Message_t), 0);
		}

		return 0xd;
	}
	else if (fake_card_state == 4) {
		log("fake_card_state 4\n")
		fake_card_state = 4;
		//d->orig_cb(pContext, NFCSTATUS_SUCCESS);
		log("%s wrapper = %x\n", __func__, wrapper.msg.eMsgType)
		// trigger callback in msgrcv(..)
		wrapper.msg.eMsgType = 0xffffff02;
		ptr_phDal4Nfc_msgsnd = msgsend;				
		if (ptr_phDal4Nfc_msgsnd != NULL) {
			ptr_phDal4Nfc_msgsnd(global_msqid, (struct msgbuf *)&wrapper, sizeof(phLibNfc_Message_t), 0);
		}

		return 0xd;
	} 

	hook_precall(&hook_phLibNfc_Ndef_CheckNdef);
	NFCSTATUS res = orig_phLibNfc_Ndef_CheckNdef(hRemoteDevice, pCheckNdef_RspCb, pContext);
	hook_postcall(&hook_phLibNfc_Ndef_CheckNdef);

	log("%s result = %x\n", __func__, res)
	return res;
}
