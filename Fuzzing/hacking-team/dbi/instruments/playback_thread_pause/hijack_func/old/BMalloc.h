void* my_BMalloc(size_t size);

struct hook_t hook_BMalloc;

/* struct special_BMalloc_t { */
/* 	//pphLibNfc_RspCb_t orig_cb; */
/* 	//pphLibNfc_RspCb_t my_cb; */
/* } special_BMalloc; */

#define HOOK_BMalloc  hook(&hook_BMalloc, pid, "libpolarisoffice", "BMalloc", my_BMalloc);

/* \ */
/* hook_BMalloc.data = &special_BMalloc;\ */
/* memset((char*)&special_BMalloc, 0, sizeof(special_BMalloc)); */
