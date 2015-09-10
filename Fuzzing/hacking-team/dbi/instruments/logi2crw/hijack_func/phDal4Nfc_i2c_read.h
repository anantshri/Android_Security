int my_phDal4Nfc_i2c_read(char * pBuffer, int nNbBytesToRead);

struct hook_t hook_phDal4Nfc_i2c_read;

struct special_phDal4Nfc_i2c_read_t {
	//pphLibNfc_RspCb_t orig_cb;
	//pphLibNfc_RspCb_t my_cb;
} special_phDal4Nfc_i2c_read;

#define HOOK_phDal4Nfc_i2c_read \
hook(&hook_phDal4Nfc_i2c_read, pid, "libnfc", "phDal4Nfc_i2c_read", my_phDal4Nfc_i2c_read);\
hook_phDal4Nfc_i2c_read.data = &special_phDal4Nfc_i2c_read;\
memset((char*)&special_phDal4Nfc_i2c_read, 0, sizeof(special_phDal4Nfc_i2c_read));
