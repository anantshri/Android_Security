#include <linux/types.h>
#include <linux/ioctl.h>

#include "utils.h"
#include "ioctls.h"
#include "ioctl_types.h"
#include "syscall.h"


static const struct ioctl jpeg0_ioctls[] = {
  IOCTL(MSM_JPEG_IOCTL_GET_HW_VERSION, STRUCT_msm_jpeg_hw_cmd, 0),
  IOCTL(MSM_JPEG_IOCTL_RESET, STRUCT_msm_jpeg_ctrl_cmd, 0),
  IOCTL(MSM_JPEG_IOCTL_STOP, STRUCT_msm_jpeg_hw_cmds, 0),
  IOCTL(MSM_JPEG_IOCTL_START, STRUCT_msm_jpeg_hw_cmds, 0),
  IOCTL(MSM_JPEG_IOCTL_INPUT_BUF_ENQUEUE, STRUCT_msm_jpeg_buf, 0),
  IOCTL(MSM_JPEG_IOCTL_INPUT_GET, STRUCT_msm_jpeg_buf, 0),
  IOCTL(MSM_JPEG_IOCTL_INPUT_GET_UNBLOCK, 0, 0),
  IOCTL(MSM_JPEG_IOCTL_OUTPUT_BUF_ENQUEUE, STRUCT_msm_jpeg_buf, 0),
  IOCTL(MSM_JPEG_IOCTL_OUTPUT_GET, STRUCT_msm_jpeg_buf, 0),
  IOCTL(MSM_JPEG_IOCTL_OUTPUT_GET_UNBLOCK, 0, 0),
  IOCTL(MSM_JPEG_IOCTL_EVT_GET, STRUCT_msm_jpeg_ctrl_cmd, 0),
  IOCTL(MSM_JPEG_IOCTL_EVT_GET_UNBLOCK, 0, 0),
  IOCTL(MSM_JPEG_IOCTL_HW_CMD, STRUCT_msm_jpeg_hw_cmd, 0),
  IOCTL(MSM_JPEG_IOCTL_HW_CMDS, STRUCT_msm_jpeg_hw_cmds, 0),
  IOCTL(MSM_JPEG_IOCTL_TEST_DUMP_REGION, 0, 0),

};

static const char *const jpeg0_devs[] = {
  "jpeg0", "msm_camera"
};

static const struct ioctl_group jpeg0_grp = {
  .devtype = DEV_MISC,
  .devs = jpeg0_devs,
  .devs_cnt = ARRAY_SIZE(jpeg0_devs),
  .sanitise = pick_random_ioctl,
  .ioctls = jpeg0_ioctls,
  .ioctls_cnt = ARRAY_SIZE(jpeg0_ioctls),
};

REG_IOCTL_GROUP(jpeg0_grp)

