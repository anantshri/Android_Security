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
#include "phLibNfc_Mgt_ConfigureDiscovery.h"

NFCSTATUS
(*orig_phLibNfc_Mgt_ConfigureDiscovery)(phLibNfc_eDiscoveryConfigMode_t DiscoveryMode,   
                        phLibNfc_sADD_Cfg_t             sADDSetup,
                        pphLibNfc_RspCb_t               pConfigDiscovery_RspCb,
                        void*                           pContext); //FIXME


void my_cb_phLibNfc_Mgt_ConfigureDiscovery(void *pContext, NFCSTATUS Status)
{
	struct special_phLibNfc_Mgt_ConfigureDiscovery_t *d = (struct special_phLibNfc_Mgt_ConfigureDiscovery_t*) hook_phLibNfc_Mgt_ConfigureDiscovery.data;
	pphLibNfc_RspCb_t cb = d->orig_cb;
	log("call %s status %x\n", __func__, Status)

	log("call %s end\n", __func__)
	cb(pContext, Status);
}

NFCSTATUS my_phLibNfc_Mgt_ConfigureDiscovery(phLibNfc_eDiscoveryConfigMode_t DiscoveryMode,   
                        phLibNfc_sADD_Cfg_t             sADDSetup,
                        pphLibNfc_RspCb_t               pConfigDiscovery_RspCb,
                        void*                           pContext) // FIXME
{
	orig_phLibNfc_Mgt_ConfigureDiscovery = (void*) hook_phLibNfc_Mgt_ConfigureDiscovery.orig;
	log("%s enter\n", __func__)
	log("orig_phLibNfc_Mgt_ConfigureDiscovery = %x\n", orig_phLibNfc_Mgt_ConfigureDiscovery)

	struct special_phLibNfc_Mgt_ConfigureDiscovery_t *d = (struct special_phLibNfc_Mgt_ConfigureDiscovery_t*)hook_phLibNfc_Mgt_ConfigureDiscovery.data;
	d->orig_cb = pConfigDiscovery_RspCb; // FIXME
	d->my_cb = my_cb_phLibNfc_Mgt_ConfigureDiscovery; // FIXME
	d->pContext = pContext;
	
	if (sADDSetup.PollDevInfo.PollEnabled != 0)
		memcpy((char*)&d->data, (char*)&sADDSetup, sizeof(sADDSetup));
	log("PollEnabled = %x\n", sADDSetup.PollDevInfo.PollEnabled);

	log("%s: pContext ptr = %x\n", __func__, pContext)
	
	hook_precall(&hook_phLibNfc_Mgt_ConfigureDiscovery);
	NFCSTATUS res = orig_phLibNfc_Mgt_ConfigureDiscovery(DiscoveryMode,sADDSetup,my_cb_phLibNfc_Mgt_ConfigureDiscovery,pContext); // FIXME
	hook_postcall(&hook_phLibNfc_Mgt_ConfigureDiscovery);

	log("%s result = %x\n", __func__, res)
	return res;
}
