#include <inttypes.h>
#include <stdlib.h>
#include <linux/ioctl.h>

#include "shm.h"
#include "utils.h"
#include "ioctls.h"
#include "ioctl_types.h"

static const struct ioctl binder_ioctls[] = {
  IOCTL(BINDER_WRITE_READ, 0, 0),
  IOCTL(BINDER_SET_IDLE_TIMEOUT, 0, 0),
  IOCTL(BINDER_SET_MAX_THREADS, 0, 0),
  IOCTL(BINDER_SET_IDLE_PRIORITY, 0, 0),
  IOCTL(BINDER_SET_CONTEXT_MGR, 0, 0),
  IOCTL(BINDER_THREAD_EXIT, 0, 0),
  IOCTL(BINDER_VERSION, 0, 0),

  IOCTL(BR_ERROR, 0, 0),
  IOCTL(BR_OK, 0, 0),
  IOCTL(BR_TRANSACTION, 0, 0),
  IOCTL(BR_REPLY, 0, 0),
  IOCTL(BR_ACQUIRE_RESULT, 0, 0),
  IOCTL(BR_DEAD_REPLY, 0, 0),
  IOCTL(BR_TRANSACTION_COMPLETE, 0, 0),
  IOCTL(BR_INCREFS, 0, 0),
  IOCTL(BR_ACQUIRE, 0, 0),
  IOCTL(BR_RELEASE, 0, 0),
  IOCTL(BR_DECREFS, 0, 0),
  IOCTL(BR_ATTEMPT_ACQUIRE, 0, 0),
  IOCTL(BR_NOOP, 0, 0),
  IOCTL(BR_SPAWN_LOOPER, 0, 0),
  IOCTL(BR_FINISHED, 0, 0),
  IOCTL(BR_DEAD_BINDER, 0, 0),
  IOCTL(BR_CLEAR_DEATH_NOTIFICATION_DONE, 0, 0),
  IOCTL(BR_FAILED_REPLY, 0, 0),

  IOCTL(BC_TRANSACTION, 0, 0),
  IOCTL(BC_REPLY, 0, 0),
  IOCTL(BC_ACQUIRE_RESULT, 0, 0),
  IOCTL(BC_FREE_BUFFER, 0, 0),
  IOCTL(BC_INCREFS, 0, 0),
  IOCTL(BC_ACQUIRE, 0, 0),
  IOCTL(BC_RELEASE, 0, 0),
  IOCTL(BC_DECREFS, 0, 0),
  IOCTL(BC_INCREFS_DONE, 0, 0),
  IOCTL(BC_ACQUIRE_DONE, 0, 0),
  IOCTL(BC_ATTEMPT_ACQUIRE, 0, 0),
  IOCTL(BC_REGISTER_LOOPER, 0, 0),
  IOCTL(BC_ENTER_LOOPER, 0, 0),
  IOCTL(BC_EXIT_LOOPER, 0, 0),
  IOCTL(BC_REQUEST_DEATH_NOTIFICATION, 0, 0),
  IOCTL(BC_CLEAR_DEATH_NOTIFICATION, 0, 0),
  IOCTL(BC_DEAD_BINDER_DONE, 0, 0),
};

static const char *const binder_miscdevs[] = {
	"binder",
};

static const struct ioctl_group binder_grp = {
	.devtype = DEV_MISC,
	.devs = binder_miscdevs,
	.devs_cnt = ARRAY_SIZE(binder_miscdevs),
	.sanitise = pick_random_ioctl,
	.ioctls = binder_ioctls,
	.ioctls_cnt = ARRAY_SIZE(binder_ioctls),
};

REG_IOCTL_GROUP(binder_grp)
