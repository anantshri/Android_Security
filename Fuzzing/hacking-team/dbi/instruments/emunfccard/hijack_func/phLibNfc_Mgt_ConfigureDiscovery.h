NFCSTATUS my_phLibNfc_Mgt_ConfigureDiscovery(phLibNfc_eDiscoveryConfigMode_t DiscoveryMode,   
                        phLibNfc_sADD_Cfg_t             sADDSetup,
                        pphLibNfc_RspCb_t               pConfigDiscovery_RspCb,
                        void*                           pContext); // FIXME

struct hook_t hook_phLibNfc_Mgt_ConfigureDiscovery;

struct special_phLibNfc_Mgt_ConfigureDiscovery_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
	phNfc_sADD_Cfg_t data;
	void*                           pContext;
} special_phLibNfc_Mgt_ConfigureDiscovery;

#define HOOK_phLibNfc_Mgt_ConfigureDiscovery \
hook(&hook_phLibNfc_Mgt_ConfigureDiscovery, pid, "libnfc", "phLibNfc_Mgt_ConfigureDiscovery", my_phLibNfc_Mgt_ConfigureDiscovery);\
hook_phLibNfc_Mgt_ConfigureDiscovery.data = &special_phLibNfc_Mgt_ConfigureDiscovery;\
memset((char*)&special_phLibNfc_Mgt_ConfigureDiscovery, 0, sizeof(special_phLibNfc_Mgt_ConfigureDiscovery));
