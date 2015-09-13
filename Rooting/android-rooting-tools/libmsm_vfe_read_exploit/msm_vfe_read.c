/*
 * Copyright (C) 2015 Hiroyuki Ikezoe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define VFE_DEVICE "/dev/v4l-subdev8"

static uint32_t vfe_base = 0;
static int device_fd = -1;
static bool initialized = false;

struct msm_vfe_cfg_cmd2 {
 uint16_t num_cfg;
 uint16_t cmd_len;
 void *cfg_data;
 void *cfg_cmd;
};

enum msm_vfe_reg_cfg_type {
  VFE_WRITE,
  VFE_WRITE_MB,
  VFE_READ,
  VFE_CFG_MASK,
  VFE_WRITE_DMI_16BIT,
  VFE_WRITE_DMI_32BIT,
  VFE_WRITE_DMI_64BIT,
  VFE_READ_DMI_16BIT,
  VFE_READ_DMI_32BIT,
  VFE_READ_DMI_64BIT,
  GET_SOC_HW_VER,
};

struct msm_vfe_reg_rw_info {
  uint32_t reg_offset;
  uint32_t cmd_data_offset;
  uint32_t len;
};

struct msm_vfe_reg_mask_info {
  uint32_t reg_offset;
  uint32_t mask;
  uint32_t val;
};

struct msm_vfe_reg_dmi_info {
  uint32_t hi_tbl_offset; /*Optional*/
  uint32_t lo_tbl_offset; /*Required*/
  uint32_t len;
};

struct msm_vfe_reg_cfg_cmd {
  union {
    struct msm_vfe_reg_rw_info rw_info;
    struct msm_vfe_reg_mask_info mask_info;
    struct msm_vfe_reg_dmi_info dmi_info;
  } u;

  enum msm_vfe_reg_cfg_type cmd_type;
};

#define BASE_VIDIOC_PRIVATE 192
#define VIDIOC_MSM_VFE_REG_CFG _IOWR('V', BASE_VIDIOC_PRIVATE, struct msm_vfe_cfg_cmd2)

static uint32_t addresses[0x100000];

#define START_OFFSET 0x2ac00000

static bool
_read(int fd, uint32_t offset, uint32_t *buffer, uint16_t size)
{
  struct msm_vfe_cfg_cmd2 cmd;
  struct msm_vfe_reg_cfg_cmd cfg_cmd;

  cmd.num_cfg = 1;
  cmd.cfg_cmd = &cfg_cmd;
  cmd.cfg_data = buffer;
  cmd.cmd_len = size;
  cfg_cmd.cmd_type = VFE_READ;

  cfg_cmd.u.rw_info.len = size;
  cfg_cmd.u.rw_info.cmd_data_offset = 0;

  cfg_cmd.u.rw_info.reg_offset = offset;
  if (ioctl(fd, VIDIOC_MSM_VFE_REG_CFG, &cmd) < 0) {
    printf("ioctl failed due to %s.\n", strerror(errno));
    return false;
  }

  return true;
}

static bool
_collect_suspicions(const char *device)
{
  int fd;
  int i, j;
  uint32_t previous_address = 0;
  struct msm_vfe_cfg_cmd2 cmd;
  struct msm_vfe_reg_cfg_cmd cfg_cmd;
  uint32_t buffer[256];

  fd = open(device, O_RDONLY);
  if (fd < 0) {
    printf("Failed to open %s due to %s.\n", device, strerror(errno));
    return false;
  }

  for (i = 0; i < 0x1000; i++) {
    if (!_read(fd, START_OFFSET + i * sizeof(buffer), buffer, sizeof(buffer))) {
      close(fd);
      return false;
    }

    for (j = 0; j < sizeof(buffer) / sizeof(uint32_t); j++) {
      if ((0xc8000000 <= buffer[j] && buffer[j] <= 0xd0000000) &&
          ((buffer[j] << 20) & 0xfff) == 0 &&
          previous_address != buffer[j]) {
        previous_address = buffer[j];
        addresses[i * sizeof(buffer) / sizeof(uint32_t)  + j] = buffer[j];
      }
    }
  }

  close(fd);

  return true;
}

#define RESOURCE_SIZE 0x2000

static uint32_t
_find_vfe_base(int fd)
{
  int i, j;
  uint32_t previous_address = 0;
  uint32_t buffer[256];
  uint32_t found_address = 0;

  for (i = 0; i < 0x1000; i++) {
    if (!_read(fd, START_OFFSET + i * sizeof(buffer), buffer, sizeof(buffer))) {
      printf("ioctl failed with %s.\n", strerror(errno));
      return 0;
    }

    for (j = 0; j < sizeof(buffer) / sizeof(uint32_t); j++) {
      if ((0xc8000000 <= buffer[j] && buffer[j] <= 0xd0000000) &&
          ((buffer[j] << 20) & 0xfff) == 0 &&
          previous_address != buffer[j]) {
        uint32_t new_offset = i * sizeof(buffer) / sizeof(uint32_t)  + j;
        if (addresses[new_offset + 0x800] == buffer[j] - RESOURCE_SIZE) {
          found_address = buffer[j];
          break;
        }
        previous_address = buffer[j];
      }
    }
  }

  return found_address;
}

static int
_open_device(const char *device)
{
  int fd;
  if (device_fd > 0) {
    return device_fd;
  }

  fd = open(device, O_RDONLY);
  if (fd < 0) {
    printf("Failed to open %s due to %s.\n", device, strerror(errno));
  }

  return fd;
}

static bool
_initialize(const char *device)
{
  if (initialized) {
    return true;
  }

  if (!_collect_suspicions(device)) {
    return false;
  }

  if (device_fd > 0) {
    close(device_fd);
    device_fd = -1;
  }

  device_fd = _open_device(device);
  if (device_fd < 0) {
    return false;
  }

  vfe_base = _find_vfe_base(device_fd);
  if (!vfe_base) {
    return false;
  }

  initialized = true;
  return true;
}

bool
msm_vfe_read_values_at_address(unsigned long address, int *value, int size)
{
  if (!initialized && !_initialize(VFE_DEVICE)) {
    return false;
  }

  return _read(device_fd, address - vfe_base, value, size);
}

bool
msm_vfe_read_value_at_address(unsigned long address, int *value)
{
  if (!initialized && !_initialize(VFE_DEVICE)) {
    return false;
  }

  return _read(device_fd, address - vfe_base, value, sizeof(*value));
}
