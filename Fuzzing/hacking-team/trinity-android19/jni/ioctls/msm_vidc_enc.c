#include <linux/types.h>
#include <linux/ioctl.h>

#include "utils.h"
#include "ioctls.h"
#include "ioctl_types.h"


static const struct ioctl msm_vidc_enc_ioctls[] = {
  IOCTL(VEN_IOCTL_SET_INTF_VERSION, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_READ_NEXT_MSG, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_STOP_READ_MSG, 0, 0),
  IOCTL(VEN_IOCTL_SET_INPUT_BUFFER_REQ, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_INPUT_BUFFER_REQ, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_ALLOC_INPUT_BUFFER, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_INPUT_BUFFER, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_FREE_INPUT_BUFFER, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_OUTPUT_BUFFER_REQ, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_OUTPUT_BUFFER_REQ, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_ALLOC_OUTPUT_BUFFER, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_OUTPUT_BUFFER, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_FREE_OUTPUT_BUFFER, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_START, 0, 0),
  IOCTL(VEN_IOCTL_CMD_ENCODE_FRAME , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_FILL_OUTPUT_BUFFER , STRUCT_venc_ioctl_msg, 0), //
  IOCTL(VEN_IOCTL_CMD_FLUSH , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_PAUSE, 0, 0),
  IOCTL(VEN_IOCTL_CMD_RESUME, 0, 0),
  IOCTL(VEN_IOCTL_CMD_STOP, 0, 0),
  IOCTL(VEN_IOCTL_SET_RECON_BUFFER  , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_FREE_RECON_BUFFER  , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_RECON_BUFFER_SIZE  , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_BASE_CFG, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_BASE_CFG, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_LIVE_MODE, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_LIVE_MODE, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_CODEC_PROFILE, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_CODEC_PROFILE, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_PROFILE_LEVEL, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_PROFILE_LEVEL, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_SHORT_HDR, STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_SHORT_HDR , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_SESSION_QP , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_SESSION_QP , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_INTRA_PERIOD , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_INTRA_PERIOD , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_CMD_REQUEST_IFRAME, 0, 0),
  IOCTL(VEN_IOCTL_GET_CAPABILITY , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_SEQUENCE_HDR , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_ENTROPY_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_ENTROPY_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_DEBLOCKING_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_DEBLOCKING_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_INTRA_REFRESH , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_INTRA_REFRESH , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_MULTI_SLICE_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_MULTI_SLICE_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_RATE_CTRL_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_RATE_CTRL_CFG  , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_VOP_TIMING_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_VOP_TIMING_CFG , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_FRAME_RATE , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_FRAME_RATE , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_TARGET_BITRATE , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_TARGET_BITRATE , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_ROTATION , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_ROTATION , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_HEC , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_HEC , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_DATA_PARTITION , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_DATA_PARTITION , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_RVLC , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_RVLC , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_AC_PREDICTION , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_AC_PREDICTION , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_QP_RANGE , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_QP_RANGE , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_NUMBER_INSTANCES , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_METABUFFER_MODE , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_EXTRADATA , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_GET_EXTRADATA , STRUCT_venc_ioctl_msg, 0),
  IOCTL(VEN_IOCTL_SET_SLICE_DELIVERY_MODE , 0, 0),
  IOCTL(VEN_IOCTL_SET_SPS_PPS_FOR_IDR, STRUCT_venc_ioctl_msg, 0)
};

static const char *const msm_vidc_enc_devs[] = {
  "msm_vidc_enc",
};

static const struct ioctl_group msm_vidc_enc_grp = {
  .devtype = DEV_MISC,
  .devs = msm_vidc_enc_devs,
  .devs_cnt = ARRAY_SIZE(msm_vidc_enc_devs),
  .sanitise = pick_random_ioctl,
  .ioctls = msm_vidc_enc_ioctls,
  .ioctls_cnt = ARRAY_SIZE(msm_vidc_enc_ioctls),
};

REG_IOCTL_GROUP(msm_vidc_enc_grp)
