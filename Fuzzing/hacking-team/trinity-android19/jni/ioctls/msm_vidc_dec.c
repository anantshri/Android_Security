#include <linux/types.h>
#include <linux/ioctl.h>

#include "utils.h"
#include "ioctls.h"
#include "ioctl_types.h"


static const struct ioctl msm_vidc_dec_ioctls[] = {
  IOCTL(VDEC_IOCTL_GET_PROFILE_LEVEL_SUPPORTED  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_INTERLACE_FORMAT , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_CURRENT_PROFILE_LEVEL  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_OUTPUT_FORMAT  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_OUTPUT_FORMAT  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_CODEC , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_CODEC , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_PICRES , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_PICRES , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_EXTRADATA , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_EXTRADATA  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_SEQUENCE_HEADER  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_BUFFER_REQ  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_BUFFER_REQ  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_ALLOCATE_BUFFER   , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_FREE_BUFFER  , STRUCT_vdec_ioctl_msg, 1),
  IOCTL(VDEC_IOCTL_SET_BUFFER  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_FILL_OUTPUT_BUFFER, STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_DECODE_FRAME  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_LOAD_RESOURCES, 0, 0),
  IOCTL(VDEC_IOCTL_CMD_START, 0, 0),
  IOCTL(VDEC_IOCTL_CMD_STOP, 0, 0),
  IOCTL(VDEC_IOCTL_CMD_PAUSE, 0, 0),
  IOCTL(VDEC_IOCTL_CMD_RESUME, 0, 0),
  IOCTL(VDEC_IOCTL_CMD_FLUSH, STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_NEXT_MSG  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_STOP_NEXT_MSG, 0, 0),
  IOCTL(VDEC_IOCTL_GET_NUMBER_INSTANCES  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_PICTURE_ORDER  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_FRAME_RATE  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_H264_MV_BUFFER  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_FREE_H264_MV_BUFFER  , STRUCT_vdec_ioctl_msg, 1),
  IOCTL(VDEC_IOCTL_GET_MV_BUFFER_SIZE  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_SET_IDR_ONLY_DECODING, 0, 0),
  IOCTL(VDEC_IOCTL_SET_CONT_ON_RECONFIG, 0, 0),
  IOCTL(VDEC_IOCTL_SET_DISABLE_DMX  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_DISABLE_DMX  , STRUCT_vdec_ioctl_msg, 0),
  IOCTL(VDEC_IOCTL_GET_DISABLE_DMX_SUPPORT , STRUCT_vdec_ioctl_msg ,0)
};

static const char *const msm_vidc_dec_ioctls_devs[] = {
  "msm_vidc_dec",
};

static const struct ioctl_group msm_vidc_dec_grp = {
  .devtype = DEV_MISC,
  .devs = msm_vidc_dec_ioctls_devs,
  .devs_cnt = ARRAY_SIZE(msm_vidc_dec_ioctls_devs),
  .sanitise = pick_random_ioctl,
  .ioctls = msm_vidc_dec_ioctls,
  .ioctls_cnt = ARRAY_SIZE(msm_vidc_dec_ioctls),
};

REG_IOCTL_GROUP(msm_vidc_dec_grp)
