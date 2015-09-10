NFCSTATUS my_phLibNfc_RemoteDev_CheckPresence( phLibNfc_Handle     hTargetDev,
                                            pphLibNfc_RspCb_t   pPresenceChk_RspCb,
                                            void*               pRspCbCtx);

struct hook_t hook_phLibNfc_RemoteDev_CheckPresence;

struct special_phLibNfc_RemoteDev_CheckPresence_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
phLibNfc_Handle     hTargetDev;
void*               pRspCbCtx;
} special_phLibNfc_RemoteDev_CheckPresence;

#define HOOK_phLibNfc_RemoteDev_CheckPresence \
hook(&hook_phLibNfc_RemoteDev_CheckPresence, pid, "libnfc", "phLibNfc_RemoteDev_CheckPresence", my_phLibNfc_RemoteDev_CheckPresence);\
hook_phLibNfc_RemoteDev_CheckPresence.data = &special_phLibNfc_RemoteDev_CheckPresence;\
memset((char*)&special_phLibNfc_RemoteDev_CheckPresence, 0, sizeof(special_phLibNfc_RemoteDev_CheckPresence));
