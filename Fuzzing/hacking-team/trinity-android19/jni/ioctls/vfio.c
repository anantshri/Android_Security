#include "config.h"

#ifdef USE_VFIO
#include <linux/vfio.h>

#include "utils.h"
#include "ioctls.h"

static const struct ioctl vfio_ioctls[] = {
  IOCTL(VFIO_GET_API_VERSION, 0, 0),
  IOCTL(VFIO_CHECK_EXTENSION, 0, 0),
  IOCTL(VFIO_SET_IOMMU, 0, 0),
  IOCTL(VFIO_GROUP_GET_STATUS, 0, 0),
  IOCTL(VFIO_GROUP_SET_CONTAINER, 0, 0),
  IOCTL(VFIO_GROUP_UNSET_CONTAINER, 0, 0),
  IOCTL(VFIO_GROUP_GET_DEVICE_FD, 0, 0),
  IOCTL(VFIO_DEVICE_GET_INFO, 0, 0),
  IOCTL(VFIO_DEVICE_GET_REGION_INFO, 0, 0),
  IOCTL(VFIO_DEVICE_GET_IRQ_INFO, 0, 0),
  IOCTL(VFIO_DEVICE_SET_IRQS, 0, 0),
  IOCTL(VFIO_DEVICE_RESET, 0, 0),
  IOCTL(VFIO_IOMMU_GET_INFO, 0, 0),
  IOCTL(VFIO_IOMMU_MAP_DMA, 0, 0),
  IOCTL(VFIO_IOMMU_UNMAP_DMA, 0, 0),
};

static const char *const vfio_devs[] = {
	"vfio",
};

static const struct ioctl_group vfio_grp = {
	.devtype = DEV_CHAR,
	.devs = vfio_devs,
	.devs_cnt = ARRAY_SIZE(vfio_devs),
	.sanitise = pick_random_ioctl,
	.ioctls = vfio_ioctls,
	.ioctls_cnt = ARRAY_SIZE(vfio_ioctls),
};

REG_IOCTL_GROUP(vfio_grp)
#endif
