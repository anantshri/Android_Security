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

NFCSTATUS (*orig_phLibNfc_RemoteDev_CheckPresence)( phLibNfc_Handle     hTargetDev,
                                            pphLibNfc_RspCb_t   pPresenceChk_RspCb,
                                            void*               pRspCbCtx
                                           );


void my_cb_phLibNfc_RemoteDev_CheckPresence(void* pContext, NFCSTATUS Status)
{
	struct special_phLibNfc_RemoteDev_CheckPresence_t *d = (struct special_phLibNfc_RemoteDev_CheckPresence_t*) hook_phLibNfc_RemoteDev_CheckPresence.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("do we have to do stuff?\n")

	log("call %s end\n", __func__)
	cb(pContext, Status);
}


NFCSTATUS my_phLibNfc_RemoteDev_CheckPresence(phLibNfc_Handle     hTargetDev,
                                            pphLibNfc_RspCb_t   pPresenceChk_RspCb,
                                            void*               pRspCbCtx
                                           )
{
	orig_phLibNfc_RemoteDev_CheckPresence = (void*) hook_phLibNfc_RemoteDev_CheckPresence.orig;
	log("%s enter\n", __func__)
	log("orig_phLibNfc_RemoteDev_CheckPresence = %x\n", orig_phLibNfc_RemoteDev_CheckPresence)

	struct special_phLibNfc_RemoteDev_CheckPresence_t *d = (struct special_phLibNfc_RemoteDev_CheckPresence_t*)hook_phLibNfc_RemoteDev_CheckPresence.data;
	d->orig_cb = pPresenceChk_RspCb; // FIXME
	d->my_cb = my_cb_phLibNfc_RemoteDev_CheckPresence; // FIXME
	d->hTargetDev = hTargetDev;
	d->pRspCbCtx = pRspCbCtx;

	log("cb for presence: %x\n",pPresenceChk_RspCb )

	if (fake_card_state > 1) { // && fake_card_state != 9) {
		//log("fake_card_state 1->2\n")
		//fake_card_state = 2;
		

		// trigger callback in msgrcv(..)
		wrapper.msg.eMsgType = 0xffffff03;
		ptr_phDal4Nfc_msgsnd = msgsend;				
		if (ptr_phDal4Nfc_msgsnd != NULL) {
			ptr_phDal4Nfc_msgsnd(global_msqid, (struct msgbuf *)&wrapper, sizeof(phLibNfc_Message_t), 0);
		}
		//pPresenceChk_RspCb(pRspCbCtx,  NFCSTATUS_SUCCESS);
		return 0xd;
	}


	hook_precall(&hook_phLibNfc_RemoteDev_CheckPresence);
	NFCSTATUS res = orig_phLibNfc_RemoteDev_CheckPresence(hTargetDev,my_cb_phLibNfc_RemoteDev_CheckPresence,pRspCbCtx);
	hook_postcall(&hook_phLibNfc_RemoteDev_CheckPresence);

	log("%s result = %x\n", __func__, res)
	return res;
}
