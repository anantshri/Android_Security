NFCSTATUS my_nfc_jni_get_technology_tree(void* e, phLibNfc_RemoteDevList_t* devList,
        uint8_t count, void* techList, void* handleList,
        void* libnfcTypeList); // FIXME

struct hook_t hook_nfc_jni_get_technology_tree;

struct special_nfc_jni_get_technology_tree_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
} special_nfc_jni_get_technology_tree;

#define HOOK_nfc_jni_get_technology_tree \
hook(&hook_nfc_jni_get_technology_tree, pid, "libnfc_jni", "_ZN7android27nfc_jni_get_technology_treeEP7_JNIEnvP18phLibNfc_RemoteDevhPP10_jintArrayS6_S6_", my_nfc_jni_get_technology_tree);\
hook_nfc_jni_get_technology_tree.data = &special_nfc_jni_get_technology_tree;\
memset((char*)&special_nfc_jni_get_technology_tree, 0, sizeof(special_nfc_jni_get_technology_tree));
