NFCSTATUS my_phLibNfc_RemoteDev_Connect(
                    phLibNfc_Handle             hRemoteDevice,
                    pphLibNfc_ConnectCallback_t pNotifyConnect_RspCb,
                    void                        *pContext
                    );

struct hook_t hook_phLibNfc_RemoteDev_Connect;

struct special_phLibNfc_RemoteDev_Connect_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
	void *pContext;
	phLibNfc_Handle             hRemoteDevice;
} special_phLibNfc_RemoteDev_Connect;

#define HOOK_phLibNfc_RemoteDev_Connect \
hook(&hook_phLibNfc_RemoteDev_Connect, pid, "libnfc", "phLibNfc_RemoteDev_Connect", my_phLibNfc_RemoteDev_Connect);\
hook_phLibNfc_RemoteDev_Connect.data = &special_phLibNfc_RemoteDev_Connect;\
memset((char*)&special_phLibNfc_RemoteDev_Connect, 0, sizeof(special_phLibNfc_RemoteDev_Connect));
