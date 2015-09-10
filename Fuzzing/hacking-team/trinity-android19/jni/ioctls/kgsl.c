#include <linux/types.h>
#include <linux/ioctl.h>

#include "utils.h"
#include "ioctls.h"
#include "ioctl_types.h"
#include "syscall.h"


static const struct ioctl kgsl_ioctls[] = {
  IOCTL(IOCTL_KGSL_DEVICE_GETPROPERTY, STRUCT_kgsl_device_getproperty, 0),
  IOCTL(IOCTL_KGSL_DEVICE_WAITTIMESTAMP, STRUCT_kgsl_device_waittimestamp, 0),
  IOCTL(IOCTL_KGSL_DEVICE_WAITTIMESTAMP_CTXTID, STRUCT_kgsl_device_waittimestamp_ctxtid, 0),
  IOCTL(IOCTL_KGSL_RINGBUFFER_ISSUEIBCMDS, STRUCT_kgsl_ringbuffer_issueibcmds, 0),
  IOCTL(IOCTL_KGSL_CMDSTREAM_READTIMESTAMP_OLD, STRUCT_kgsl_cmdstream_readtimestamp, 0),
  IOCTL(IOCTL_KGSL_CMDSTREAM_READTIMESTAMP, STRUCT_kgsl_cmdstream_readtimestamp, 0),
  IOCTL(IOCTL_KGSL_CMDSTREAM_FREEMEMONTIMESTAMP, STRUCT_kgsl_cmdstream_freememontimestamp, 0),
  IOCTL(IOCTL_KGSL_CMDSTREAM_FREEMEMONTIMESTAMP_OLD, STRUCT_kgsl_cmdstream_freememontimestamp, 0),
  IOCTL(IOCTL_KGSL_DRAWCTXT_CREATE, STRUCT_kgsl_drawctxt_create, 0),
  IOCTL(IOCTL_KGSL_DRAWCTXT_DESTROY, STRUCT_kgsl_drawctxt_destroy, 0),
  IOCTL(IOCTL_KGSL_MAP_USER_MEM, STRUCT_kgsl_map_user_mem, 0),
  IOCTL(IOCTL_KGSL_CMDSTREAM_READTIMESTAMP_CTXTID, STRUCT_kgsl_cmdstream_readtimestamp_ctxtid, 0),
  IOCTL(IOCTL_KGSL_CMDSTREAM_FREEMEMONTIMESTAMP_CTXTID, STRUCT_kgsl_cmdstream_freememontimestamp_ctxtid, 0),
  IOCTL(IOCTL_KGSL_SHAREDMEM_FROM_PMEM, STRUCT_kgsl_sharedmem_from_pmem, 0),
  IOCTL(IOCTL_KGSL_SHAREDMEM_FREE, STRUCT_kgsl_sharedmem_free, 0),
  IOCTL(IOCTL_KGSL_CFF_USER_EVENT, STRUCT_kgsl_cff_user_event, 0),
  IOCTL(IOCTL_KGSL_DRAWCTXT_BIND_GMEM_SHADOW, STRUCT_kgsl_bind_gmem_shadow, 0),
  IOCTL(IOCTL_KGSL_SHAREDMEM_FROM_VMALLOC, STRUCT_kgsl_sharedmem_from_vmalloc, 0),
  IOCTL(IOCTL_KGSL_SHAREDMEM_FLUSH_CACHE, STRUCT_kgsl_sharedmem_free , 0),
  IOCTL(IOCTL_KGSL_DRAWCTXT_SET_BIN_BASE_OFFSET, STRUCT_kgsl_drawctxt_set_bin_base_offset , 0),
  IOCTL(IOCTL_KGSL_CMDWINDOW_WRITE, STRUCT_kgsl_cmdwindow_write , 0),
  IOCTL(IOCTL_KGSL_GPUMEM_ALLOC, STRUCT_kgsl_gpumem_alloc , 0),
  IOCTL(IOCTL_KGSL_CFF_SYNCMEM, STRUCT_kgsl_cff_syncmem , 0),
  IOCTL(IOCTL_KGSL_TIMESTAMP_EVENT_OLD, STRUCT_kgsl_timestamp_event , 0),
  IOCTL(IOCTL_KGSL_SETPROPERTY, STRUCT_kgsl_device_getproperty , 0),
  IOCTL(IOCTL_KGSL_TIMESTAMP_EVENT, STRUCT_kgsl_timestamp_event , 0),
  IOCTL(IOCTL_KGSL_GPUMEM_ALLOC_ID, STRUCT_kgsl_gpumem_alloc_id , 0),
  IOCTL(IOCTL_KGSL_GPUMEM_FREE_ID, STRUCT_kgsl_gpumem_free_id , 0),
  IOCTL(IOCTL_KGSL_GPUMEM_GET_INFO, STRUCT_kgsl_gpumem_get_info , 0),
  IOCTL(IOCTL_KGSL_GPUMEM_SYNC_CACHE, STRUCT_kgsl_gpumem_sync_cache , 0),
  IOCTL(IOCTL_KGSL_PERFCOUNTER_GET, STRUCT_kgsl_perfcounter_get , 0),
  IOCTL(IOCTL_KGSL_PERFCOUNTER_PUT, STRUCT_kgsl_perfcounter_put , 0),
  IOCTL(IOCTL_KGSL_PERFCOUNTER_QUERY, STRUCT_kgsl_perfcounter_query , 0),
  IOCTL(IOCTL_KGSL_PERFCOUNTER_READ, STRUCT_kgsl_perfcounter_read , 0),
};

static const char *const kgsl_devs[] = {
  "kgsl",
};

static const struct ioctl_group kgsl_grp = {
  .devtype = DEV_MISC,
  .devs = kgsl_devs,
  .devs_cnt = ARRAY_SIZE(kgsl_devs),
  .sanitise = pick_random_ioctl,
  .ioctls = kgsl_ioctls,
  .ioctls_cnt = ARRAY_SIZE(kgsl_ioctls),
};

REG_IOCTL_GROUP(kgsl_grp)

