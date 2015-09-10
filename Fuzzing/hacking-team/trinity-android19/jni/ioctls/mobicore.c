#include <linux/types.h>
#include <linux/ioctl.h>

#include "utils.h"
#include "ioctls.h"
#include "ioctl_types.h"
#include "syscall.h"


static const struct ioctl mobicore_ioctls[] = {
  IOCTL(MC_IO_INIT, STRUCT_mc_ioctl_init, 0),
  IOCTL(MC_IO_INFO, STRUCT_mc_ioctl_info, 0),
  IOCTL(MC_IO_VERSION, 0, 0),
  IOCTL(MC_IO_YIELD, 0, 0),
  IOCTL(MC_IO_NSIQ, 0, 0),
  IOCTL(MC_IO_FREE, 0, 0),
  IOCTL(MC_IO_REG_WSM, STRUCT_mc_ioctl_reg_wsm, 0),
  IOCTL(MC_IO_UNREG_WSM, 0, 0),
  IOCTL(MC_IO_LOCK_WSM, 0, 0),
  IOCTL(MC_IO_UNLOCK_WSM, 0, 0),
  IOCTL(MC_IO_EXECUTE, STRUCT_mc_ioctl_execute, 0),
  IOCTL(MC_IO_MAP_WSM, STRUCT_mc_ioctl_map, 0),
  IOCTL(MC_IO_MAP_MCI, STRUCT_mc_ioctl_map, 0),
  IOCTL(MC_IO_MAP_PWSM, STRUCT_mc_ioctl_map, 0),
  IOCTL(MC_IO_CLEAN_WSM, 0, 0),
  IOCTL(MC_IO_RESOLVE_WSM, STRUCT_mc_ioctl_resolv_wsm, 0),
  IOCTL(MC_IO_RESOLVE_CONT_WSM, STRUCT_mc_ioctl_resolv_cont_wsm, 0),
  IOCTL(MC_IO_LOG_SETUP, 0, 0),
};

static const char *const mobicore_devs[] = {
  "mobicore",
};

static const struct ioctl_group mobicore_grp = {
  .devtype = DEV_MISC,
  .devs = mobicore_devs,
  .devs_cnt = ARRAY_SIZE(mobicore_devs),
  .sanitise = pick_random_ioctl,
  .ioctls = mobicore_ioctls,
  .ioctls_cnt = ARRAY_SIZE(mobicore_ioctls),
};

REG_IOCTL_GROUP(mobicore_grp)

