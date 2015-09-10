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
(*orig_phLibNfc_RemoteDev_Connect)(                    phLibNfc_Handle             hRemoteDevice,
                    pphLibNfc_ConnectCallback_t pNotifyConnect_RspCb,
                    void                        *pContext
                    );
/*
void my_cb_phLibNfc_RemoteDev_Connect(...) // FIXME
{
	struct special_phLibNfc_RemoteDev_Connect_t *d = (struct special_phLibNfc_RemoteDev_Connect_t*) hook_phLibNfc_RemoteDev_Connect.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

extern phNfc_sRemoteDevInformation_t *di;

NFCSTATUS my_phLibNfc_RemoteDev_Connect(                    phLibNfc_Handle             hRemoteDevice,
                    pphLibNfc_ConnectCallback_t pNotifyConnect_RspCb,
                    void                        *pContext)
{
	orig_phLibNfc_RemoteDev_Connect = (void*) hook_phLibNfc_RemoteDev_Connect.orig;
	log("%s enter\n", __func__)
	log("orig_phLibNfc_RemoteDev_Connect = %x\n", orig_phLibNfc_RemoteDev_Connect)

	struct special_phLibNfc_RemoteDev_Connect_t *d = (struct special_phLibNfc_RemoteDev_Connect_t*)hook_phLibNfc_RemoteDev_Connect.data;
	d->orig_cb = pNotifyConnect_RspCb; // FIXME
	d->my_cb = NULL; //my_cb_phLibNfc_RemoteDev_Connect; // FIXME
	d->pContext = pContext;
	d->hRemoteDevice = hRemoteDevice;
	
	if (fake_card_state >= 1) {
		log("fake_card_state 1->2\n")
		if (fake_card_state < 2) fake_card_state = 2;
		
		log("%s wrapper = %x\n", __func__, wrapper.msg.eMsgType)
		// trigger callback in msgrcv(..)
		wrapper.msg.eMsgType = 0xffffff01;
		ptr_phDal4Nfc_msgsnd = msgsend;				
		if (ptr_phDal4Nfc_msgsnd != NULL) {
			ptr_phDal4Nfc_msgsnd(global_msqid, (struct msgbuf *)&wrapper, sizeof(phLibNfc_Message_t), 0);
		}
		
		//pNotifyConnect_RspCb(pContext, hRemoteDevice, di, NFCSTATUS_SUCCESS);
		return 0xd;
	}

	log("%s: pContext ptr = %x\n", __func__, pContext)
	
	hook_precall(&hook_phLibNfc_RemoteDev_Connect);
	NFCSTATUS res = orig_phLibNfc_RemoteDev_Connect(hRemoteDevice,pNotifyConnect_RspCb,pContext);
	hook_postcall(&hook_phLibNfc_RemoteDev_Connect);

	log("%s result = %x\n", __func__, res)
	return res;
}
