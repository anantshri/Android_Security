NFCSTATUS my_-h(); // FIXME

struct hook_t hook_-h;

struct special_-h_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
} special_-h;

#define HOOK_-h \
hook(&hook_-h, pid, "libnfc", "-h", my_-h);\
hook_-h.data = &special_-h;\
memset((char*)&special_-h, 0, sizeof(special_-h));