#include <linux/ioctl.h>
#include <linux/loop.h>

#include "utils.h"
#include "ioctls.h"

#define LOOP_SET_CAPACITY       0x4C07

/* Intentionally mix the loop and loop-control ioctls. */
static const struct ioctl loop_ioctls[] = {
  IOCTL(LOOP_SET_FD, 0, 0),
  IOCTL(LOOP_CLR_FD, 0, 0),
  IOCTL(LOOP_SET_STATUS, 0, 0),
  IOCTL(LOOP_GET_STATUS, 0, 0),
  IOCTL(LOOP_SET_STATUS64, 0, 0),
  IOCTL(LOOP_GET_STATUS64, 0, 0),
  IOCTL(LOOP_CHANGE_FD, 0, 0),
  IOCTL(LOOP_SET_CAPACITY, 0, 0),

#ifdef LOOP_CTL_ADD
  IOCTL(LOOP_CTL_ADD, 0, 0),
#endif
#ifdef LOOP_CTL_REMOVE
  IOCTL(LOOP_CTL_REMOVE, 0, 0),
#endif
#ifdef LOOP_CTL_GET_FREE
  IOCTL(LOOP_CTL_GET_FREE, 0, 0),
#endif
};

static const char *const loop_ctrl_devs[] = {
	"loop-control",
	//FIXME: Need to glob /dev/loop*
};

static const struct ioctl_group loop_ctrl_grp = {
	.devtype = DEV_MISC,
	.devs = loop_ctrl_devs,
	.devs_cnt = ARRAY_SIZE(loop_ctrl_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = loop_ioctls,
	.ioctls_cnt = ARRAY_SIZE(loop_ioctls),
};

REG_IOCTL_GROUP(loop_ctrl_grp)

static const char *const loop_devs[] = {
	"loop",
};

static const struct ioctl_group loop_grp = {
	.devtype = DEV_BLOCK,
	.devs = loop_devs,
	.devs_cnt = ARRAY_SIZE(loop_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = loop_ioctls,
	.ioctls_cnt = ARRAY_SIZE(loop_ioctls),
};

REG_IOCTL_GROUP(loop_grp)
