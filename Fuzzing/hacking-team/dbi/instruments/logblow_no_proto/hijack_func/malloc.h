void* my_malloc(size_t size); // FIXME

struct hook_t hook_malloc;

struct special_malloc_t {
  //	pphLibNfc_RspCb_t orig_cb;
  //	pphLibNfc_RspCb_t my_cb;
} special_malloc;

#define HOOK_malloc \
hook(&hook_malloc, pid, "libc", "malloc", my_malloc);\
hook_malloc.data = &special_malloc;\
memset((char*)&special_malloc, 0, sizeof(special_malloc));
