int my_blow(); 

struct hook_t hook_blow;

struct special_blow_t {
	/* pphLibNfc_RspCb_t orig_cb; */
	/* pphLibNfc_RspCb_t my_cb; */
} special_blow;

#define HOOK_blow \
  hook(&hook_blow, pid, "libtest_lib", "blow", my_blow, 0, 0xf71 ); \
hook_blow.data = &special_blow;\
memset((char*)&special_blow, 0, sizeof(special_blow));
