#include <linux/types.h>
#include <linux/ioctl.h>

#include "utils.h"
#include "ioctls.h"

/*
 * Ioctl definitions
 */

/* Use 'k' as magic number */
#define HX280ENC_IOC_MAGIC  'k'
/*
 * S means "Set" through a ptr,
 * T means "Tell" directly with the argument value
 * G means "Get": reply by setting through a pointer
 * Q means "Query": response is on the return value
 * X means "eXchange": G and S atomically
 * H means "sHift": T and Q atomically
 */

#define HX280ENC_IOCGHWOFFSET      _IOR(HX280ENC_IOC_MAGIC,  1, unsigned long *)
#define HX280ENC_IOCGHWIOSIZE      _IOR(HX280ENC_IOC_MAGIC,  2, unsigned int *)
#define HX280ENC_IOC_CLI           _IO(HX280ENC_IOC_MAGIC,  3)
#define HX280ENC_IOC_STI           _IO(HX280ENC_IOC_MAGIC,  4)

#define HX280ENC_IOCHARDRESET      _IO(HX280ENC_IOC_MAGIC, 5)   /* debugging tool */
#define HX280ENC_IOCVIRTBASE       _IO(HX280ENC_IOC_MAGIC, 6)
#define HX280ENC_IOCWAITDONE       _IO(HX280ENC_IOC_MAGIC, 7)
#define HX280ENC_IOC_CLOCK_ON      _IO(HX280ENC_IOC_MAGIC, 8)
#define HX280ENC_IOC_CLOCK_OFF     _IO(HX280ENC_IOC_MAGIC, 9)

#define HX280ENC_IOC_MAXNR 9

static const struct ioctl hx280enc_ioctls[] = {
  IOCTL(HX280ENC_IOCGHWOFFSET, 0, 0),
  IOCTL(HX280ENC_IOCGHWIOSIZE, 0, 0),
  IOCTL(HX280ENC_IOC_CLI, 0, 0),
  IOCTL(HX280ENC_IOC_STI, 0, 0),
  IOCTL(HX280ENC_IOCHARDRESET, 0, 0),
  IOCTL(HX280ENC_IOCVIRTBASE, 0, 0),
  IOCTL(HX280ENC_IOCWAITDONE, 0, 0),
  IOCTL(HX280ENC_IOC_CLOCK_ON, 0, 0),
  IOCTL(HX280ENC_IOC_CLOCK_OFF, 0, 0)
};

static const char *const hx280enc_devs[] = {
  "hx280enc",
};

static const struct ioctl_group hx280enc_grp = {
  .devtype = DEV_MISC,
  .devs = hx280enc_devs,
  .devs_cnt = ARRAY_SIZE(hx280enc_devs),
  .sanitise = pick_random_ioctl,
  .ioctls = hx280enc_ioctls,
  .ioctls_cnt = ARRAY_SIZE(hx280enc_ioctls),
};

REG_IOCTL_GROUP(hx280enc_grp)
