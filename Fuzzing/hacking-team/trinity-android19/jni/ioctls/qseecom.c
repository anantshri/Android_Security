#include <linux/types.h>
#include <linux/ioctl.h>

#include "utils.h"
#include "ioctls.h"
#include "ioctl_types.h"
#include "syscall.h"


static const struct ioctl qseecom_ioctls[] = {
  IOCTL(QSEECOM_IOCTL_REGISTER_LISTENER_REQ, STRUCT_qseecom_register_listener_req, 0),
  IOCTL(QSEECOM_IOCTL_UNREGISTER_LISTENER_REQ, 0, 0),
  IOCTL(QSEECOM_IOCTL_SEND_CMD_REQ, STRUCT_qseecom_send_cmd_req, 0),
  IOCTL(QSEECOM_IOCTL_SEND_MODFD_CMD_REQ, STRUCT_qseecom_send_modfd_cmd_req, 0),
  IOCTL(QSEECOM_IOCTL_RECEIVE_REQ, 0, 0),
  IOCTL(QSEECOM_IOCTL_SEND_RESP_REQ, 0, 0),
  IOCTL(QSEECOM_IOCTL_LOAD_APP_REQ, STRUCT_qseecom_load_img_req, 0),
  IOCTL(QSEECOM_IOCTL_SET_MEM_PARAM_REQ, STRUCT_qseecom_set_sb_mem_param_req, 0),
  IOCTL(QSEECOM_IOCTL_UNLOAD_APP_REQ, 0, 0),
  IOCTL(QSEECOM_IOCTL_GET_QSEOS_VERSION_REQ, STRUCT_qseecom_qseos_version_req, 0),
  IOCTL(QSEECOM_IOCTL_PERF_ENABLE_REQ, 0, 0),
  IOCTL(QSEECOM_IOCTL_PERF_DISABLE_REQ, 0, 0),
  IOCTL(QSEECOM_IOCTL_LOAD_EXTERNAL_ELF_REQ, STRUCT_qseecom_load_img_req, 0),
  IOCTL(QSEECOM_IOCTL_UNLOAD_EXTERNAL_ELF_REQ, 0, 0),
  IOCTL(QSEECOM_IOCTL_APP_LOADED_QUERY_REQ, STRUCT_qseecom_qseos_app_load_query, 0)
};

static const char *const qseecom_devs[] = {
  "qseecom",
};

static const struct ioctl_group qseecom_grp = {
  .devtype = DEV_MISC,
  .devs = qseecom_devs,
  .devs_cnt = ARRAY_SIZE(qseecom_devs),
  .sanitise = pick_random_ioctl,
  .ioctls = qseecom_ioctls,
  .ioctls_cnt = ARRAY_SIZE(qseecom_ioctls),
};

REG_IOCTL_GROUP(qseecom_grp)

