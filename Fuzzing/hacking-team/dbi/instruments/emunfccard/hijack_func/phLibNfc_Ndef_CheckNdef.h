NFCSTATUS my_phLibNfc_Ndef_CheckNdef(phLibNfc_Handle       hRemoteDevice,
                        pphLibNfc_ChkNdefRspCb_t        pCheckNdef_RspCb,
                        void*                           pContext);

struct hook_t hook_phLibNfc_Ndef_CheckNdef;

struct special_phLibNfc_Ndef_CheckNdef_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
	void *pContext;
} special_phLibNfc_Ndef_CheckNdef;

#define HOOK_phLibNfc_Ndef_CheckNdef \
hook(&hook_phLibNfc_Ndef_CheckNdef, pid, "libnfc", "phLibNfc_Ndef_CheckNdef", my_phLibNfc_Ndef_CheckNdef);\
hook_phLibNfc_Ndef_CheckNdef.data = &special_phLibNfc_Ndef_CheckNdef;\
memset((char*)&special_phLibNfc_Ndef_CheckNdef, 0, sizeof(special_phLibNfc_Ndef_CheckNdef));
