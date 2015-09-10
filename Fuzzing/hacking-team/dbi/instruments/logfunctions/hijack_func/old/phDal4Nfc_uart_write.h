int my_phDal4Nfc_uart_write(unsigned char *pBuffer, int nNbBytesToWrite);

struct hook_t hook_phDal4Nfc_uart_write;

struct special_phDal4Nfc_uart_write_t {
	//pphLibNfc_RspCb_t orig_cb;
	//pphLibNfc_RspCb_t my_cb;
} special_phDal4Nfc_uart_write;

#define HOOK_phDal4Nfc_uart_write \
hook(&hook_phDal4Nfc_uart_write, pid, "libnfc", "phDal4Nfc_uart_write", my_phDal4Nfc_uart_write);\
hook_phDal4Nfc_uart_write.data = &special_phDal4Nfc_uart_write;\
memset((char*)&special_phDal4Nfc_uart_write, 0, sizeof(special_phDal4Nfc_uart_write));
