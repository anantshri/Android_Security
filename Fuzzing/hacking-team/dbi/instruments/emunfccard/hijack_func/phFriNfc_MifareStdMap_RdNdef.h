#define HOOK_phFriNfc_MifareStdMap_RdNdef \
hook(&hook_phFriNfc_MifareStdMap_RdNdef, pid, "libnfc", "phFriNfc_MifareStdMap_RdNdef", my_phFriNfc_MifareStdMap_RdNdef);\
hook_phFriNfc_MifareStdMap_RdNdef.data = &special_phFriNfc_MifareStdMap_RdNdef;\
memset((char*)&special_phFriNfc_MifareStdMap_RdNdef, 0, sizeof(special_phFriNfc_MifareStdMap_RdNdef));

