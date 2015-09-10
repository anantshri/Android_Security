#include <fcntl.h>
//#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "utils.h"
#include "ioctls.h"
#include "ioctl_types.h"
#include "syscall.h"

static const struct ioctl videodev2_ioctls[] = {
  IOCTL(VIDIOC_QUERYCAP, STRUCT_v4l2_capability, 0),
  IOCTL(VIDIOC_RESERVED, 0, 0),
  IOCTL(VIDIOC_ENUM_FMT, STRUCT_v4l2_fmtdesc, 0),
  IOCTL(VIDIOC_G_FMT, STRUCT_v4l2_format, 0),
  IOCTL(VIDIOC_S_FMT, STRUCT_v4l2_format, 0),
  IOCTL(VIDIOC_REQBUFS, STRUCT_v4l2_requestbuffers, 0),
  IOCTL(VIDIOC_QUERYBUF, STRUCT_v4l2_buffer, 0),
  IOCTL(VIDIOC_G_FBUF, STRUCT_v4l2_framebuffer, 0),
  IOCTL(VIDIOC_S_FBUF, STRUCT_v4l2_framebuffer, 0),
  IOCTL(VIDIOC_OVERLAY, 0, 0),
  IOCTL(VIDIOC_QBUF, STRUCT_v4l2_buffer, 0),
#ifdef VIDIOC_EXPBUF
  IOCTL(VIDIOC_EXPBUF, 0, 0),
#endif
  IOCTL(VIDIOC_DQBUF, STRUCT_v4l2_buffer, 0),
  IOCTL(VIDIOC_STREAMON, 0, 0),
  IOCTL(VIDIOC_STREAMOFF, 0, 0),
  IOCTL(VIDIOC_G_PARM, STRUCT_v4l2_streamparm, 0),
  IOCTL(VIDIOC_S_PARM, STRUCT_v4l2_streamparm, 0),
  IOCTL(VIDIOC_G_STD, 0, 0),
  IOCTL(VIDIOC_S_STD, 0, 0),
  IOCTL(VIDIOC_ENUMSTD, STRUCT_v4l2_standard, 0),
  IOCTL(VIDIOC_ENUMINPUT, STRUCT_v4l2_input, 0),
  IOCTL(VIDIOC_G_CTRL, STRUCT_v4l2_control, 0),
  IOCTL(VIDIOC_S_CTRL, STRUCT_v4l2_control, 0),
  IOCTL(VIDIOC_G_TUNER, STRUCT_v4l2_tuner, 0),
  IOCTL(VIDIOC_S_TUNER, STRUCT_v4l2_tuner, 0),
  IOCTL(VIDIOC_G_AUDIO, STRUCT_v4l2_audio, 0),
  IOCTL(VIDIOC_S_AUDIO, STRUCT_v4l2_audio, 0),
  IOCTL(VIDIOC_QUERYCTRL, STRUCT_v4l2_queryctrl, 0),
  IOCTL(VIDIOC_QUERYMENU, STRUCT_v4l2_querymenu, 0),
  IOCTL(VIDIOC_G_INPUT, 0, 0),
  IOCTL(VIDIOC_S_INPUT, 0, 0),
  IOCTL(VIDIOC_G_OUTPUT, 0, 0),
  IOCTL(VIDIOC_S_OUTPUT, 0, 0),
  IOCTL(VIDIOC_ENUMOUTPUT, STRUCT_v4l2_output, 0),
  IOCTL(VIDIOC_G_AUDOUT, STRUCT_v4l2_audioout, 0),
  IOCTL(VIDIOC_S_AUDOUT, STRUCT_v4l2_audioout, 0),
  IOCTL(VIDIOC_G_MODULATOR, STRUCT_v4l2_modulator, 0),
  IOCTL(VIDIOC_S_MODULATOR, STRUCT_v4l2_modulator, 0),
  IOCTL(VIDIOC_G_FREQUENCY, STRUCT_v4l2_frequency, 0),
  IOCTL(VIDIOC_S_FREQUENCY, STRUCT_v4l2_frequency, 0),
  IOCTL(VIDIOC_CROPCAP, STRUCT_v4l2_cropcap, 0),
  IOCTL(VIDIOC_G_CROP, STRUCT_v4l2_crop, 0),
  IOCTL(VIDIOC_S_CROP, STRUCT_v4l2_crop, 0),
  IOCTL(VIDIOC_G_JPEGCOMP, STRUCT_v4l2_jpegcompression, 0),
  IOCTL(VIDIOC_S_JPEGCOMP, STRUCT_v4l2_jpegcompression, 0),
  IOCTL(VIDIOC_QUERYSTD, 0, 0),
  IOCTL(VIDIOC_TRY_FMT, STRUCT_v4l2_format, 0),
  IOCTL(VIDIOC_ENUMAUDIO, STRUCT_v4l2_audio, 0),
  IOCTL(VIDIOC_ENUMAUDOUT, STRUCT_v4l2_audioout, 0),
  IOCTL(VIDIOC_G_PRIORITY, 0, 0),
  IOCTL(VIDIOC_S_PRIORITY, 0, 0),
  IOCTL(VIDIOC_G_SLICED_VBI_CAP, STRUCT_v4l2_sliced_vbi_cap, 0),
  IOCTL(VIDIOC_LOG_STATUS, 0, 0),
  IOCTL(VIDIOC_G_EXT_CTRLS, STRUCT_v4l2_ext_controls, 0),
  IOCTL(VIDIOC_S_EXT_CTRLS, STRUCT_v4l2_ext_controls, 0),
  IOCTL(VIDIOC_TRY_EXT_CTRLS, STRUCT_v4l2_ext_controls, 0),
  

#define ASD 1
#ifdef ASD // LOLLIPOP 
  IOCTL(VIDIOC_ENUM_FRAMESIZES, STRUCT_v4l2_frmsizeenum, 0),
  IOCTL(VIDIOC_ENUM_FRAMEINTERVALS, STRUCT_v4l2_frmivalenum, 0),
  IOCTL(VIDIOC_G_ENC_INDEX, STRUCT_v4l2_enc_idx, 0),
  IOCTL(VIDIOC_ENCODER_CMD, STRUCT_v4l2_encoder_cmd, 0),
  IOCTL(VIDIOC_TRY_ENCODER_CMD, STRUCT_v4l2_encoder_cmd, 0),
  IOCTL(VIDIOC_DBG_S_REGISTER, STRUCT_v4l2_dbg_register, 0),
  IOCTL(VIDIOC_DBG_G_REGISTER, STRUCT_v4l2_dbg_register, 0),
#endif
#ifdef VIDIOC_DBG_G_CHIP_IDENT
  IOCTL(VIDIOC_DBG_G_CHIP_IDENT, STRUCT_v4l2_dbg_chip_ident, 0),
#endif
#ifdef VIDIOC_S_HW_FREQ_SEEK
  IOCTL(VIDIOC_S_HW_FREQ_SEEK, STRUCT_v4l2_hw_freq_seek, 0),
#endif
#ifdef VIDIOC_ENUM_DV_PRESETS
  IOCTL(VIDIOC_ENUM_DV_PRESETS, STRUCT_v4l2_dv_enum_preset, 0),
#endif
#ifdef VIDIOC_S_DV_PRESET
  IOCTL(VIDIOC_S_DV_PRESET, STRUCT_v4l2_dv_preset, 0),
#endif
#ifdef VIDIOC_G_DV_PRESET
  IOCTL(VIDIOC_G_DV_PRESET, STRUCT_v4l2_dv_preset, 0),
#endif
#ifdef VIDIOC_QUERY_DV_PRESET
  IOCTL(VIDIOC_QUERY_DV_PRESET, STRUCT_v4l2_dv_preset, 0),
#endif
#ifdef VIDIOC_S_DV_TIMINGS
  IOCTL(VIDIOC_S_DV_TIMINGS, STRUCT_v4l2_dv_timings, 0),
#endif
#ifdef VIDIOC_G_DV_TIMINGS
  IOCTL(VIDIOC_G_DV_TIMINGS, STRUCT_v4l2_dv_timings, 0),
#endif
#ifdef VIDIOC_DQEVENT
  IOCTL(VIDIOC_DQEVENT, STRUCT_v4l2_event, 0),
#endif
#ifdef VIDIOC_SUBSCRIBE_EVENT
  IOCTL(VIDIOC_SUBSCRIBE_EVENT, STRUCT_v4l2_event_subscription, 0),
#endif
#ifdef VIDIOC_UNSUBSCRIBE_EVENT
  IOCTL(VIDIOC_UNSUBSCRIBE_EVENT, STRUCT_v4l2_event_subscription, 0),
#endif
#ifdef VIDIOC_CREATE_BUFS
  IOCTL(VIDIOC_CREATE_BUFS, STRUCT_v4l2_create_buffers, 0),
#endif
#ifdef VIDIOC_PREPARE_BUF
  IOCTL(VIDIOC_PREPARE_BUF, STRUCT_v4l2_buffer, 0),
#endif
#ifdef VIDIOC_G_SELECTION
  IOCTL(VIDIOC_G_SELECTION, STRUCT_v4l2_selection, 0),
#endif
#ifdef VIDIOC_S_SELECTION
  IOCTL(VIDIOC_S_SELECTION, STRUCT_v4l2_selection, 0),
#endif
#ifdef VIDIOC_DECODER_CMD
  IOCTL(VIDIOC_DECODER_CMD, STRUCT_v4l2_decoder_cmd, 0),
#endif
#ifdef VIDIOC_TRY_DECODER_CMD
  IOCTL(VIDIOC_TRY_DECODER_CMD, STRUCT_v4l2_decoder_cmd, 0),
#endif
#ifdef VIDIOC_ENUM_DV_TIMINGS
  IOCTL(VIDIOC_ENUM_DV_TIMINGS, 0, 0),
#endif
#ifdef VIDIOC_QUERY_DV_TIMINGS
  IOCTL(VIDIOC_QUERY_DV_TIMINGS, 0, 0),
#endif
#ifdef VIDIOC_DV_TIMINGS_CAP
  IOCTL(VIDIOC_DV_TIMINGS_CAP, 0, 0),
#endif
#ifdef VIDIOC_ENUM_FREQ_BANDS
  IOCTL(VIDIOC_ENUM_FREQ_BANDS, 0, 0),
#endif
};

static const char *const videodev2_devs[] = {
	"video4linux",
};

static const struct ioctl_group videodev2_grp = {
	.devtype = DEV_MISC,
	.devs = videodev2_devs,
	.devs_cnt = ARRAY_SIZE(videodev2_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = videodev2_ioctls,
	.ioctls_cnt = ARRAY_SIZE(videodev2_ioctls),
};

REG_IOCTL_GROUP(videodev2_grp)
