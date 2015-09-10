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

//#include "phDal4Nfc_msgrcv.h"
#include "phDal4Nfc_messageQueueLib.h"

NFCSTATUS (*orig_phDal4Nfc_msgrcv)(int msqid, void * msgp, size_t msgsz, long msgtyp, int msgflg);

/*
void my_cb_phDal4Nfc_msgrcv(...) // FIXME
{
	struct special_phDal4Nfc_msgrcv_t *d = (struct special_phDal4Nfc_msgrcv_t*) hook_phDal4Nfc_msgrcv.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}
*/

extern void ifc_call_cb();
extern phNfc_sRemoteDevInformation_t *di;

//int global_msqid = NULL;

void my_nfc_jni_discover_callback(void *pContext, NFCSTATUS status)
{
    log("%s called\n", __func__)
	NFCSTATUS (*orig_phLibNfc_Mgt_ConfigureDiscovery)(phLibNfc_eDiscoveryConfigMode_t DiscoveryMode,   
                        phLibNfc_sADD_Cfg_t             sADDSetup,
                        pphLibNfc_RspCb_t               pConfigDiscovery_RspCb,
                        void*                           pContext); //FIXME
						
	orig_phLibNfc_Mgt_ConfigureDiscovery = (void*) hook_phLibNfc_Mgt_ConfigureDiscovery.orig;
	struct special_phLibNfc_Mgt_ConfigureDiscovery_t *d = (struct special_phLibNfc_Mgt_ConfigureDiscovery_t*)hook_phLibNfc_Mgt_ConfigureDiscovery.data;
	phLibNfc_sADD_Cfg_t discovery_cfg;
	discovery_cfg.PollDevInfo.PollEnabled = 0x3f;
	discovery_cfg.Duration = 300000; /* in ms */
	discovery_cfg.NfcIP_Mode = phNfc_eDefaultP2PMode;
	discovery_cfg.NfcIP_Tgt_Disable = TRUE;
	wrapper.msg.eMsgType = 0xffffff06;
	sleep(2);
	ptr_phDal4Nfc_msgsnd = msgsend;
	ptr_phDal4Nfc_msgsnd(global_msqid, (struct msgbuf *)&wrapper, sizeof(phLibNfc_Message_t), 0);
/*
	hook_precall(&hook_phLibNfc_Mgt_ConfigureDiscovery);
	NFCSTATUS res = orig_phLibNfc_Mgt_ConfigureDiscovery(NFC_DISCOVERY_CONFIG, discovery_cfg, d->my_cb, d->pContext);
	hook_postcall(&hook_phLibNfc_Mgt_ConfigureDiscovery);
	log("enabled discovery %x\n", res)
*/
}

NFCSTATUS my_phDal4Nfc_msgrcv(int msqid, void * msgp, size_t msgsz, long msgtyp, int msgflg) // FIXME
{
	orig_phDal4Nfc_msgrcv = (void*) hook_phDal4Nfc_msgrcv.orig;
	//log(" %s enter\n", __func__)
	//log(" orig_phDal4Nfc_msgrcv = %x\n", orig_phDal4Nfc_msgrcv)

	struct special_phDal4Nfc_msgrcv_t *d = (struct special_phDal4Nfc_msgrcv_t*)hook_phDal4Nfc_msgrcv.data;
	d->orig_cb = NULL; // FIXME
	//d->my_cb = my_cb_phDal4Nfc_msgrcv; // FIXME
	//d->pContext = pContext;

	if (global_msqid == 0) {
		global_msqid = msqid;
		log(" msqid = %x\n", msqid)
	}

	//log(" msg rcv sleep\n")
	hook_precall(&hook_phDal4Nfc_msgrcv);
	status_wait = 1;
	NFCSTATUS res = orig_phDal4Nfc_msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
	status_wait = 0;
	hook_postcall(&hook_phDal4Nfc_msgrcv);
	//log(" wokeup\n")

	phDal4Nfc_Message_Wrapper_t *wrapper;
	wrapper = (phDal4Nfc_Message_Wrapper_t*) msgp;
	if (wrapper->msg.eMsgType == 0xffffff00) {
		log(" wrapper 0x00 call ifc\n")
		wrapper->msg.eMsgType = 0x0;
		//fake_card_state = 1;
		ifc_call_cb();
	}
	else if (wrapper->msg.eMsgType == 0xffffff01) {
		log(" wrapper 0x01 dev_connect\n")
		wrapper->msg.eMsgType = 0x0;
		// check remote connect
		struct special_phLibNfc_RemoteDev_Connect_t *d = (struct special_phLibNfc_RemoteDev_Connect_t*)hook_phLibNfc_RemoteDev_Connect.data;
		pphLibNfc_ConnectCallback_t  cb = d->orig_cb;
		
		cb(d->pContext, d->hRemoteDevice, di, NFCSTATUS_SUCCESS);
	}
	else if (wrapper->msg.eMsgType == 0xffffff02) {
		wrapper->msg.eMsgType = 0x0;
		log(" wrapper 0x02 checkNdef\n")
		// checkndef
		struct special_phLibNfc_Ndef_CheckNdef_t *d = (struct special_phLibNfc_Ndef_CheckNdef_t*)hook_phLibNfc_Ndef_CheckNdef.data;
		d->orig_cb(d->pContext, NFCSTATUS_SUCCESS);
		
	}
	else if (wrapper->msg.eMsgType == 0xffffff03) {
		log(" wrapper 0x03 checkPresence\n")
		wrapper->msg.eMsgType = 0x0;		
		struct special_phLibNfc_RemoteDev_CheckPresence_t *d = (struct special_phLibNfc_RemoteDev_CheckPresence_t*)hook_phLibNfc_RemoteDev_CheckPresence.data;
		d->orig_cb(d->pRspCbCtx, NFCSTATUS_SUCCESS);
	}
	else if (wrapper->msg.eMsgType == 0xffffff05) {
		log(" wrapper 0x05 trigger\n")
		wrapper->msg.eMsgType = 0x0;		
		NFCSTATUS
(*orig_phLibNfc_Mgt_ConfigureDiscovery)(phLibNfc_eDiscoveryConfigMode_t DiscoveryMode,   
                        phLibNfc_sADD_Cfg_t             sADDSetup,
                        pphLibNfc_RspCb_t               pConfigDiscovery_RspCb,
                        void*                           pContext); //FIXME
		orig_phLibNfc_Mgt_ConfigureDiscovery = (void*) hook_phLibNfc_Mgt_ConfigureDiscovery.orig;
		struct special_phLibNfc_Mgt_ConfigureDiscovery_t *d = (struct special_phLibNfc_Mgt_ConfigureDiscovery_t*)hook_phLibNfc_Mgt_ConfigureDiscovery.data;
		phLibNfc_sADD_Cfg_t discovery_cfg;
		discovery_cfg.PollDevInfo.PollEnabled = 0;
		discovery_cfg.Duration = 300000; /* in ms */
		discovery_cfg.NfcIP_Mode = phNfc_eDefaultP2PMode;
		discovery_cfg.NfcIP_Tgt_Disable = TRUE;

		hook_precall(&hook_phLibNfc_Mgt_ConfigureDiscovery);
		NFCSTATUS res = orig_phLibNfc_Mgt_ConfigureDiscovery(NFC_DISCOVERY_CONFIG,discovery_cfg, my_nfc_jni_discover_callback,d->pContext);
		hook_postcall(&hook_phLibNfc_Mgt_ConfigureDiscovery);
		log("disabled discovery %x\n", res)
	}
	else if (wrapper->msg.eMsgType == 0xffffff06) {
		log(" wrapper 0x06 trigger\n")
		wrapper->msg.eMsgType = 0x0;		
		NFCSTATUS
(*orig_phLibNfc_Mgt_ConfigureDiscovery)(phLibNfc_eDiscoveryConfigMode_t DiscoveryMode,   
                        phLibNfc_sADD_Cfg_t             sADDSetup,
                        pphLibNfc_RspCb_t               pConfigDiscovery_RspCb,
                        void*                           pContext); //FIXME
		orig_phLibNfc_Mgt_ConfigureDiscovery = (void*) hook_phLibNfc_Mgt_ConfigureDiscovery.orig;
		struct special_phLibNfc_Mgt_ConfigureDiscovery_t *d = (struct special_phLibNfc_Mgt_ConfigureDiscovery_t*)hook_phLibNfc_Mgt_ConfigureDiscovery.data;
		phLibNfc_sADD_Cfg_t discovery_cfg;
		discovery_cfg.PollDevInfo.PollEnabled = 0x3f;
		discovery_cfg.Duration = 300000; /* in ms */
		discovery_cfg.NfcIP_Mode = phNfc_eDefaultP2PMode;
		discovery_cfg.NfcIP_Tgt_Disable = FALSE;
		sleep(2);

		log("%x\n", d->data.PollDevInfo.PollEnabled);
		hook_precall(&hook_phLibNfc_Mgt_ConfigureDiscovery);
		NFCSTATUS res = orig_phLibNfc_Mgt_ConfigureDiscovery(NFC_DISCOVERY_CONFIG,d->data, d->my_cb,d->pContext);
		hook_postcall(&hook_phLibNfc_Mgt_ConfigureDiscovery);
		log("enable discovery %x\n", res)
	}

	//log(" %s result = %x\n", __func__, res)
	return res;
}
