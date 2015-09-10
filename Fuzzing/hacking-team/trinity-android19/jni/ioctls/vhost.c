#include "config.h"

#ifdef USE_VHOST
#include <linux/vhost.h>

#include "utils.h"
#include "ioctls.h"

static const struct ioctl vhost_ioctls[] = {
  IOCTL(VHOST_GET_FEATURES, 0, 0),
  IOCTL(VHOST_SET_FEATURES, 0, 0),
  IOCTL(VHOST_SET_OWNER, 0, 0),
  IOCTL(VHOST_RESET_OWNER, 0, 0),
  IOCTL(VHOST_SET_MEM_TABLE, 0, 0),
  IOCTL(VHOST_SET_LOG_BASE, 0, 0),
  IOCTL(VHOST_SET_LOG_FD, 0, 0),
  IOCTL(VHOST_SET_VRING_NUM, 0, 0),
  IOCTL(VHOST_SET_VRING_ADDR, 0, 0),
  IOCTL(VHOST_SET_VRING_BASE, 0, 0),
  IOCTL(VHOST_GET_VRING_BASE, 0, 0),
  IOCTL(VHOST_SET_VRING_KICK, 0, 0),
  IOCTL(VHOST_SET_VRING_CALL, 0, 0),
  IOCTL(VHOST_SET_VRING_ERR, 0, 0),
  IOCTL(VHOST_NET_SET_BACKEND, 0, 0),
#ifdef VHOST_SCSI_SET_ENDPOINT
  IOCTL(VHOST_SCSI_SET_ENDPOINT, 0, 0),
#endif
#ifdef VHOST_SCSI_CLEAR_ENDPOINT
  IOCTL(VHOST_SCSI_CLEAR_ENDPOINT, 0, 0),
#endif
#ifdef VHOST_SCSI_GET_ABI_VERSION
  IOCTL(VHOST_SCSI_GET_ABI_VERSION, 0, 0),
#endif
#ifdef VHOST_SCSI_SET_EVENTS_MISSED
  IOCTL(VHOST_SCSI_SET_EVENTS_MISSED, 0, 0),
#endif
#ifdef VHOST_SCSI_GET_EVENTS_MISSED
  IOCTL(VHOST_SCSI_GET_EVENTS_MISSED, 0, 0),
#endif
};

static const char *const vhost_devs[] = {
	"vhost-net",
};

static const struct ioctl_group vhost_grp = {
	.devtype = DEV_MISC,
	.devs = vhost_devs,
	.devs_cnt = ARRAY_SIZE(vhost_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = vhost_ioctls,
	.ioctls_cnt = ARRAY_SIZE(vhost_ioctls),
};

REG_IOCTL_GROUP(vhost_grp)
#endif /* USE_VHOST */
