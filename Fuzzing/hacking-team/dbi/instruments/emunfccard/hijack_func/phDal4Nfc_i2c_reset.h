int my_phDal4Nfc_i2c_reset(long level); 

struct hook_t hook_phDal4Nfc_i2c_reset;

struct special_phDal4Nfc_i2c_reset_t {
	pphLibNfc_RspCb_t orig_cb;
	pphLibNfc_RspCb_t my_cb;
} special_phDal4Nfc_i2c_reset;

#define HOOK_phDal4Nfc_i2c_reset \
hook(&hook_phDal4Nfc_i2c_reset, pid, "libnfc", "phDal4Nfc_i2c_reset", my_phDal4Nfc_i2c_reset);\
hook_phDal4Nfc_i2c_reset.data = &special_phDal4Nfc_i2c_reset;\
memset((char*)&special_phDal4Nfc_i2c_reset, 0, sizeof(special_phDal4Nfc_i2c_reset));
