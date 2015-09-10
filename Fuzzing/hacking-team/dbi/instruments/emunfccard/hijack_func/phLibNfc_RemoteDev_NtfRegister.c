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
#include "phLibNfc_RemoteDev_NtfRegister.h"

NFCSTATUS   
(*orig_phLibNfc_RemoteDev_NtfRegister)( 
                        phLibNfc_Registry_Info_t*       pRegistryInfo,
                        phLibNfc_NtfRegister_RspCb_t    pNotificationHandler,
                        void                            *pContext
                        );

extern phNfc_sRemoteDevInformation_t *di;
extern phLibNfc_RemoteDevList_t *rdevl;

void my_cb_phLibNfc_RemoteDev_NtfRegister(void*                           pContext,
    phLibNfc_RemoteDevList_t*       psRemoteDevList,
    uint8_t                         uNofRemoteDev,
    NFCSTATUS                       Status)
{
	struct special_phLibNfc_RemoteDev_NtfRegister_t *d = (struct special_phLibNfc_RemoteDev_NtfRegister_t*) hook_phLibNfc_RemoteDev_NtfRegister.data;
	phLibNfc_NtfRegister_RspCb_t cb = d->orig_cb;
	log("call %s\n", __func__)
	log("cb = %x\n", cb)

	int i;
	
	if (d->RemoteDevList == NULL)
		d->RemoteDevList = psRemoteDevList;
	
	log("%s: pcontext ptr = %x\n", __func__, pContext)
	log("%s: uNofRemoteDev = %d\n", __func__, uNofRemoteDev);
	log("%s: Status = %x\n", __func__, Status);
	log("%s: devlistptr = %x\n", __func__, psRemoteDevList);
	log("%s: devinfoptr = %x\n", __func__, psRemoteDevList->psRemoteDevInfo);
	log("    hTragetDev = %x\n", __func__, psRemoteDevList->hTargetDev);
	log("    session %x  remdevtype %x\n", 
		psRemoteDevList->psRemoteDevInfo->SessionOpened,
		psRemoteDevList->psRemoteDevInfo->RemDevType);
	
	log("uuid length = %d id=", psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info.UidLength)
	for (i = 0; i < PHHAL_MAX_UID_LENGTH; i++)
		log("%0.2x", psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info.Uid[i])
	log("\n")
	log("appdata length = %d id=", psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info.AppDataLength)
	for (i = 0; i < PHHAL_MAX_ATR_LENGTH; i++)
		log("%0.2x", psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info.AppData[i])
	log("\n")
	log("Sak %x\n",psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info.Sak)
	log("atq=")
	for (i = 0; i < PHHAL_ATQA_LENGTH; i++)
		log("%0.2x", psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info.AtqA[i])
	log("\n")
	log("MaxDataRate %x\n",psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info.MaxDataRate)
	log("Fwi_Sfgt %x\n",psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info.Fwi_Sfgt)

	if (di == NULL) {
		log("fill my di\n")
		// store for later
		di = malloc(sizeof(phNfc_sRemoteDevInformation_t));
		memcpy((char*)&di->RemoteDevInfo.Iso14443A_Info, 
			(char*)&psRemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info, 
			sizeof(struct phNfc_sIso14443AInfo_t));
		rdevl = malloc(sizeof(phLibNfc_RemoteDevList_t));
		rdevl->hTargetDev = psRemoteDevList->hTargetDev;
		rdevl->psRemoteDevInfo = di;
		di->SessionOpened = psRemoteDevList->psRemoteDevInfo->SessionOpened;
		di->RemDevType = psRemoteDevList->psRemoteDevInfo->RemDevType;
	}
	log("call %s end\n", __func__)
	cb(pContext, psRemoteDevList, uNofRemoteDev, Status);
}


NFCSTATUS my_phLibNfc_RemoteDev_NtfRegister( 
                        phLibNfc_Registry_Info_t*       pRegistryInfo,
                        phLibNfc_NtfRegister_RspCb_t    pNotificationHandler,
                        void                            *pContext
                        )
{
	orig_phLibNfc_RemoteDev_NtfRegister = (void*) hook_phLibNfc_RemoteDev_NtfRegister.orig;
	log("%s enter\n", __func__)
	log("orig_phLibNfc_RemoteDev_NtfRegister = %x\n", orig_phLibNfc_RemoteDev_NtfRegister)

	struct special_phLibNfc_RemoteDev_NtfRegister_t *d = (struct special_phLibNfc_RemoteDev_NtfRegister_t*)hook_phLibNfc_RemoteDev_NtfRegister.data;
	d->orig_cb = pNotificationHandler;
	d->my_cb = my_cb_phLibNfc_RemoteDev_NtfRegister;

	d->pRegistryInfo = pRegistryInfo;
	d->orig_cb = pNotificationHandler;
	d->my_cb = my_cb_phLibNfc_RemoteDev_NtfRegister;
	d->pContext = pContext;

	log("%s: pContext ptr = %x\n", __func__, pContext)

	hook_precall(&hook_phLibNfc_RemoteDev_NtfRegister);
	NFCSTATUS res = orig_phLibNfc_RemoteDev_NtfRegister(pRegistryInfo,my_cb_phLibNfc_RemoteDev_NtfRegister,pContext);
	hook_postcall(&hook_phLibNfc_RemoteDev_NtfRegister);

	log("%s result = %x\n", __func__, res)
	return res;
}
