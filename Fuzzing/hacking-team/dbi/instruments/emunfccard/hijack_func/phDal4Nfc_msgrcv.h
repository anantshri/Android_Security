NFCSTATUS my_phDal4Nfc_msgrcv(int msqid, void * msgp, size_t msgsz, long msgtyp, int msgflg); // FIXME

struct hook_t hook_phDal4Nfc_msgrcv;

int (*ptr_phDal4Nfc_msgsnd)(int msqid, void * msgp, size_t msgsz, int msgflg);

struct special_phDal4Nfc_msgrcv_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
} special_phDal4Nfc_msgrcv;

#define HOOK_phDal4Nfc_msgrcv \
hook(&hook_phDal4Nfc_msgrcv, pid, "libnfc", "phDal4Nfc_msgrcv", my_phDal4Nfc_msgrcv);\
hook_phDal4Nfc_msgrcv.data = &special_phDal4Nfc_msgrcv;\
memset((char*)&special_phDal4Nfc_msgrcv, 0, sizeof(special_phDal4Nfc_msgrcv));
