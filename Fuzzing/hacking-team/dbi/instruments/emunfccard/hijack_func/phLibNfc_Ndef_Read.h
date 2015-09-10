NFCSTATUS my_phLibNfc_Ndef_Read( phLibNfc_Handle                   hRemoteDevice,
                            phNfc_sData_t                      *psRd,
                            phLibNfc_Ndef_EOffset_t             Offset,
                            pphLibNfc_RspCb_t                   pNdefRead_RspCb,
                            void*                               pContext);

struct hook_t hook_phLibNfc_Ndef_Read;

struct special_phLibNfc_Ndef_Read_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
phNfc_sData_t                      *psRd;
	void *pContext;
	phLibNfc_Ndef_EOffset_t Offset;
} special_phLibNfc_Ndef_Read;


#define HOOK_phLibNfc_Ndef_Read \
hook(&hook_phLibNfc_Ndef_Read, pid, "libnfc", "phLibNfc_Ndef_Read", my_phLibNfc_Ndef_Read);\
hook_phLibNfc_Ndef_Read.data = &special_phLibNfc_Ndef_Read;\
memset((char*)&special_phLibNfc_Ndef_Read, 0, sizeof(special_phLibNfc_Ndef_Read));
