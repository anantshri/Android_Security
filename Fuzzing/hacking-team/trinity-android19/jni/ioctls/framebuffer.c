#include <linux/ioctl.h>
#include <linux/fb.h>

#include "utils.h"
#include "ioctls.h"

static const struct ioctl fb_ioctls[] = {
  IOCTL(FBIOGET_VSCREENINFO, 0, 0),
  IOCTL(FBIOPUT_VSCREENINFO, 0, 0),
  IOCTL(FBIOGET_FSCREENINFO, 0, 0),
  IOCTL(FBIOGETCMAP, 0, 0),
  IOCTL(FBIOPUTCMAP, 0, 0),
  IOCTL(FBIOPAN_DISPLAY, 0, 0),
  IOCTL(FBIO_CURSOR, 0, 0),
  IOCTL(FBIOGET_CON2FBMAP, 0, 0),
  IOCTL(FBIOPUT_CON2FBMAP, 0, 0),
  IOCTL(FBIOBLANK, 0, 0),
  IOCTL(FBIOGET_VBLANK, 0, 0),
  IOCTL(FBIO_ALLOC, 0, 0),
  IOCTL(FBIO_FREE, 0, 0),
  IOCTL(FBIOGET_GLYPH, 0, 0),
  IOCTL(FBIOGET_HWCINFO, 0, 0),
  IOCTL(FBIOPUT_MODEINFO, 0, 0),
  IOCTL(FBIOGET_DISPINFO, 0, 0),
#ifdef FBIO_WAITFORVSYNC
  IOCTL(FBIO_WAITFORVSYNC, 0, 0),
#endif
};

static const char *const fb_chardevs[] = {
	"fb",
};

static const struct ioctl_group fb_grp = {
	.devtype = DEV_CHAR,
	.devs = fb_chardevs,
	.devs_cnt = ARRAY_SIZE(fb_chardevs),
	.sanitise = pick_random_ioctl,
	.ioctls = fb_ioctls,
	.ioctls_cnt = ARRAY_SIZE(fb_ioctls),
};

REG_IOCTL_GROUP(fb_grp)
