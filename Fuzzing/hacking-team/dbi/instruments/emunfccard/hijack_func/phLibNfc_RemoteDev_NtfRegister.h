NFCSTATUS   
my_phLibNfc_RemoteDev_NtfRegister( 
                        phLibNfc_Registry_Info_t*       pRegistryInfo,
                        phLibNfc_NtfRegister_RspCb_t    pNotificationHandler,
                        void                            *pContext
                        );


struct hook_t hook_phLibNfc_RemoteDev_NtfRegister;

struct special_phLibNfc_RemoteDev_NtfRegister_t {
	phLibNfc_NtfRegister_RspCb_t orig_cb;
	phLibNfc_NtfRegister_RspCb_t my_cb;
	phLibNfc_Registry_Info_t*       pRegistryInfo;
	void                            *pContext;
	phLibNfc_RemoteDevList_t*       RemoteDevList;
} special_phLibNfc_RemoteDev_NtfRegister;

#define HOOK_phLibNfc_RemoteDev_NtfRegister \
hook(&hook_phLibNfc_RemoteDev_NtfRegister, pid, "libnfc", "phLibNfc_RemoteDev_NtfRegister", my_phLibNfc_RemoteDev_NtfRegister);\
hook_phLibNfc_RemoteDev_NtfRegister.data = &special_phLibNfc_RemoteDev_NtfRegister;\
memset((char*)&special_phLibNfc_RemoteDev_NtfRegister, 0, sizeof(special_phLibNfc_RemoteDev_NtfRegister));
