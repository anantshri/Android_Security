#include <linux/ioctl.h>
#include <linux/rtc.h>

#include "ioctls.h"
#include "utils.h"

static const struct ioctl rtc_ioctls[] = {
  IOCTL(RTC_AIE_ON, 0, 0),
  IOCTL(RTC_AIE_OFF, 0, 0),
  IOCTL(RTC_UIE_ON, 0, 0),
  IOCTL(RTC_UIE_OFF, 0, 0),
  IOCTL(RTC_PIE_ON, 0, 0),
  IOCTL(RTC_PIE_OFF, 0, 0),
  IOCTL(RTC_WIE_ON, 0, 0),
  IOCTL(RTC_WIE_OFF, 0, 0),
  IOCTL(RTC_ALM_SET, 0, 0),
  IOCTL(RTC_ALM_READ, 0, 0),
  IOCTL(RTC_RD_TIME, 0, 0),
  IOCTL(RTC_SET_TIME, 0, 0),
  IOCTL(RTC_IRQP_READ, 0, 0),
  IOCTL(RTC_IRQP_SET, 0, 0),
  IOCTL(RTC_EPOCH_READ, 0, 0),
  IOCTL(RTC_EPOCH_SET, 0, 0),
  IOCTL(RTC_WKALM_SET, 0, 0),
  IOCTL(RTC_WKALM_RD, 0, 0),
  IOCTL(RTC_PLL_GET, 0, 0),
  IOCTL(RTC_PLL_SET, 0, 0),
#ifdef RTC_VL_READ
  IOCTL(RTC_VL_READ, 0, 0),
#endif
#ifdef RTC_VL_CLR
  IOCTL(RTC_VL_CLR, 0, 0),
#endif
};

static const char *const rtc_devs[] = {
	"rtc",
};

static const struct ioctl_group rtc_grp = {
	.devtype = DEV_CHAR,
	.devs = rtc_devs,
	.devs_cnt = ARRAY_SIZE(rtc_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = rtc_ioctls,
	.ioctls_cnt = ARRAY_SIZE(rtc_ioctls),
};

REG_IOCTL_GROUP(rtc_grp)
