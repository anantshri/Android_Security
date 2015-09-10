#include <linux/types.h>
#include <linux/ioctl.h>
#include <inttypes.h>
#include <stdlib.h>

/********************************/
/**** QSEECOM *******************/
/********************************/

#define MAX_ION_FD  4
#define MAX_APP_NAME_SIZE  32

/*
 * struct qseecom_register_listener_req -
 *      for register listener ioctl request
 * @listener_id - service id (shared between userspace and QSE)
 * @ifd_data_fd - ion handle
 * @virt_sb_base - shared buffer base in user space
 * @sb_size - shared buffer size
 */
struct qseecom_register_listener_req {
  uint32_t listener_id; /* in */
  int32_t ifd_data_fd; /* in */
  uint32_t virt_sb_base; /* in */
  uint32_t sb_size; /* in */
};

/*
 * struct qseecom_send_cmd_req - for send command ioctl request
 * @cmd_req_len - command buffer length
 * @cmd_req_buf - command buffer
 * @resp_len - response buffer length
 * @resp_buf - response buffer
 */
struct qseecom_send_cmd_req {
  void *cmd_req_buf; /* in */
  unsigned int cmd_req_len; /* in */
  void *resp_buf; /* in/out */
  unsigned int resp_len; /* in/out */
};


/*
 * struct qseecom_ion_fd_info - ion fd handle data information
 * @fd - ion handle to some memory allocated in user space
 * @cmd_buf_offset - command buffer offset
 */
struct qseecom_ion_fd_info {
  int32_t fd;
  uint32_t cmd_buf_offset;
};
/*
 * struct qseecom_send_modfd_cmd_req - for send command ioctl request
 * @cmd_req_len - command buffer length
 * @cmd_req_buf - command buffer
 * @resp_len - response buffer length
 * @resp_buf - response buffer
 * @ifd_data_fd - ion handle to memory allocated in user space
 * @cmd_buf_offset - command buffer offset
 */
struct qseecom_send_modfd_cmd_req {
  void *cmd_req_buf; /* in */
  unsigned int cmd_req_len; /* in */
  void *resp_buf; /* in/out */
  unsigned int resp_len; /* in/out */
  struct qseecom_ion_fd_info ifd_data[MAX_ION_FD];
};
/*
 * struct qseecom_listener_send_resp_req - signal to continue the send_cmd req.
 * Used as a trigger from HLOS service to notify QSEECOM that it's done with its
 * operation and provide the response for QSEECOM can continue the incomplete
 * command execution
 * @resp_len - Length of the response
 * @resp_buf - Response buffer where the response of the cmd should go.
 */
struct qseecom_send_resp_req {
  void *resp_buf; /* in */
  unsigned int resp_len; /* in */
};

/*
 * struct qseecom_load_img_data - for sending image length information and
 * ion file descriptor to the qseecom driver. ion file descriptor is used
 * for retrieving the ion file handle and in turn the physical address of
 * the image location.
 * @mdt_len - Length of the .mdt file in bytes.
 * @img_len - Length of the .mdt + .b00 +..+.bxx images files in bytes
 * @ion_fd - Ion file descriptor used when allocating memory.
 * @img_name - Name of the image.
 */
struct qseecom_load_img_req {
  uint32_t mdt_len; /* in */
  uint32_t img_len; /* in */
  int32_t  ifd_data_fd; /* in */
  char img_name[MAX_APP_NAME_SIZE]; /* in */
  int app_id; /* out*/
};

struct qseecom_set_sb_mem_param_req {
  int32_t ifd_data_fd; /* in */
  uint32_t virt_sb_base; /* in */
  uint32_t sb_len; /* in */
};

/*
 * struct qseecom_qseos_version_req - get qseos version
 * @qseos_version - version number
 */
struct qseecom_qseos_version_req {
  unsigned int qseos_version; /* in */
};

/*
 * struct qseecom_qseos_app_load_query - verify if app is loaded in qsee
 * @app_name[MAX_APP_NAME_SIZE]-  name of the app.
 * @app_id - app id.
 */
struct qseecom_qseos_app_load_query {
  char app_name[MAX_APP_NAME_SIZE]; /* in */
  int app_id; /* out */
};

#define QSEECOM_IOC_MAGIC    0x97



#define QSEECOM_IOCTL_REGISTER_LISTENER_REQ \
  _IOWR(QSEECOM_IOC_MAGIC, 1, struct qseecom_register_listener_req)

#define QSEECOM_IOCTL_UNREGISTER_LISTENER_REQ \
  _IO(QSEECOM_IOC_MAGIC, 2)

#define QSEECOM_IOCTL_SEND_CMD_REQ \
  _IOWR(QSEECOM_IOC_MAGIC, 3, struct qseecom_send_cmd_req)

#define QSEECOM_IOCTL_SEND_MODFD_CMD_REQ \
  _IOWR(QSEECOM_IOC_MAGIC, 4, struct qseecom_send_modfd_cmd_req)

#define QSEECOM_IOCTL_RECEIVE_REQ \
  _IO(QSEECOM_IOC_MAGIC, 5)

#define QSEECOM_IOCTL_SEND_RESP_REQ \
  _IO(QSEECOM_IOC_MAGIC, 6)

#define QSEECOM_IOCTL_LOAD_APP_REQ \
  _IOWR(QSEECOM_IOC_MAGIC, 7, struct qseecom_load_img_req)

#define QSEECOM_IOCTL_SET_MEM_PARAM_REQ \
  _IOWR(QSEECOM_IOC_MAGIC, 8, struct qseecom_set_sb_mem_param_req)

#define QSEECOM_IOCTL_UNLOAD_APP_REQ \
  _IO(QSEECOM_IOC_MAGIC, 9)

#define QSEECOM_IOCTL_GET_QSEOS_VERSION_REQ \
  _IOWR(QSEECOM_IOC_MAGIC, 10, struct qseecom_qseos_version_req)

#define QSEECOM_IOCTL_PERF_ENABLE_REQ \
  _IO(QSEECOM_IOC_MAGIC, 11)

#define QSEECOM_IOCTL_PERF_DISABLE_REQ \
  _IO(QSEECOM_IOC_MAGIC, 12)

#define QSEECOM_IOCTL_LOAD_EXTERNAL_ELF_REQ \
  _IOWR(QSEECOM_IOC_MAGIC, 13, struct qseecom_load_img_req)

#define QSEECOM_IOCTL_UNLOAD_EXTERNAL_ELF_REQ \
  _IO(QSEECOM_IOC_MAGIC, 14)

#define QSEECOM_IOCTL_APP_LOADED_QUERY_REQ \
  _IOWR(QSEECOM_IOC_MAGIC, 15, struct qseecom_qseos_app_load_query)



/***********************************/
/******* BINDER ********************/
/***********************************/


#define __user
/* drivers/staging/android/binder.h */
/*
 * Copyright (C) 2008 Google, Inc.
 *
 * Based on, but no longer compatible with, the original
 * OpenBinder.org binder driver interface, which is:
 *
 * Copyright (c) 2005 Palmsource, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#define B_PACK_CHARS(c1, c2, c3, c4) \
	((((c1)<<24)) | (((c2)<<16)) | (((c3)<<8)) | (c4))
#define B_TYPE_LARGE 0x85

enum {
	BINDER_TYPE_BINDER	= B_PACK_CHARS('s', 'b', '*', B_TYPE_LARGE),
	BINDER_TYPE_WEAK_BINDER	= B_PACK_CHARS('w', 'b', '*', B_TYPE_LARGE),
	BINDER_TYPE_HANDLE	= B_PACK_CHARS('s', 'h', '*', B_TYPE_LARGE),
	BINDER_TYPE_WEAK_HANDLE	= B_PACK_CHARS('w', 'h', '*', B_TYPE_LARGE),
	BINDER_TYPE_FD		= B_PACK_CHARS('f', 'd', '*', B_TYPE_LARGE),
};

enum {
	FLAT_BINDER_FLAG_PRIORITY_MASK = 0xff,
	FLAT_BINDER_FLAG_ACCEPTS_FDS = 0x100,
};

/*
 * This is the flattened representation of a Binder object for transfer
 * between processes.  The 'offsets' supplied as part of a binder transaction
 * contains offsets into the data where these structures occur.  The Binder
 * driver takes care of re-writing the structure type and data as it moves
 * between processes.
 */
struct flat_binder_object {
	/* 8 bytes for large_flat_header. */
	unsigned long		type;
	unsigned long		flags;

	/* 8 bytes of data. */
	union {
		void __user	*binder;	/* local object */
		signed long	handle;		/* remote object */
	};

	/* extra data associated with local object */
	void __user		*cookie;
};

/*
 * On 64-bit platforms where user code may run in 32-bits the driver must
 * translate the buffer (and local binder) addresses appropriately.
 */

struct binder_write_read {
	signed long	write_size;	/* bytes to write */
	signed long	write_consumed;	/* bytes consumed by driver */
	unsigned long	write_buffer;
	signed long	read_size;	/* bytes to read */
	signed long	read_consumed;	/* bytes consumed by driver */
	unsigned long	read_buffer;
};

/* Use with BINDER_VERSION, driver fills in fields. */
struct binder_version {
	/* driver protocol version -- increment with incompatible change */
	signed long	protocol_version;
};

/* This is the current protocol version. */
#define BINDER_CURRENT_PROTOCOL_VERSION 7

#define BINDER_WRITE_READ		_IOWR('b', 1, struct binder_write_read)
#define	BINDER_SET_IDLE_TIMEOUT		_IOW('b', 3, int64_t)
#define	BINDER_SET_MAX_THREADS		_IOW('b', 5, size_t)
#define	BINDER_SET_IDLE_PRIORITY	_IOW('b', 6, int)
#define	BINDER_SET_CONTEXT_MGR		_IOW('b', 7, int)
#define	BINDER_THREAD_EXIT		_IOW('b', 8, int)
#define BINDER_VERSION			_IOWR('b', 9, struct binder_version)

/*
 * NOTE: Two special error codes you should check for when calling
 * in to the driver are:
 *
 * EINTR -- The operation has been interupted.  This should be
 * handled by retrying the ioctl() until a different error code
 * is returned.
 *
 * ECONNREFUSED -- The driver is no longer accepting operations
 * from your process.  That is, the process is being destroyed.
 * You should handle this by exiting from your process.  Note
 * that once this error code is returned, all further calls to
 * the driver from any thread will return this same code.
 */

enum transaction_flags {
	TF_ONE_WAY	= 0x01,	/* this is a one-way call: async, no return */
	TF_ROOT_OBJECT	= 0x04,	/* contents are the component's root object */
	TF_STATUS_CODE	= 0x08,	/* contents are a 32-bit status code */
	TF_ACCEPT_FDS	= 0x10,	/* allow replies with file descriptors */
};

struct binder_transaction_data {
	/* The first two are only used for bcTRANSACTION and brTRANSACTION,
	 * identifying the target and contents of the transaction.
	 */
	union {
		size_t	handle;	/* target descriptor of command transaction */
		void	*ptr;	/* target descriptor of return transaction */
	} target;
	void		*cookie;	/* target object cookie */
	unsigned int	code;		/* transaction command */

	/* General information about the transaction. */
	unsigned int	flags;
	pid_t		sender_pid;
	uid_t		sender_euid;
	size_t		data_size;	/* number of bytes of data */
	size_t		offsets_size;	/* number of bytes of offsets */

	/* If this transaction is inline, the data immediately
	 * follows here; otherwise, it ends with a pointer to
	 * the data buffer.
	 */
	union {
		struct {
			/* transaction data */
			const void __user	*buffer;
			/* offsets from buffer to flat_binder_object structs */
			const void __user	*offsets;
		} ptr;
		uint8_t	buf[8];
	} data;
};

struct binder_ptr_cookie {
	void *ptr;
	void *cookie;
};

struct binder_pri_desc {
	int priority;
	int desc;
};

struct binder_pri_ptr_cookie {
	int priority;
	void *ptr;
	void *cookie;
};

enum BinderDriverReturnProtocol {
	BR_ERROR = _IOR('r', 0, int),
	/*
	 * int: error code
	 */

	BR_OK = _IO('r', 1),
	/* No parameters! */

	BR_TRANSACTION = _IOR('r', 2, struct binder_transaction_data),
	BR_REPLY = _IOR('r', 3, struct binder_transaction_data),
	/*
	 * binder_transaction_data: the received command.
	 */

	BR_ACQUIRE_RESULT = _IOR('r', 4, int),
	/*
	 * not currently supported
	 * int: 0 if the last bcATTEMPT_ACQUIRE was not successful.
	 * Else the remote object has acquired a primary reference.
	 */

	BR_DEAD_REPLY = _IO('r', 5),
	/*
	 * The target of the last transaction (either a bcTRANSACTION or
	 * a bcATTEMPT_ACQUIRE) is no longer with us.  No parameters.
	 */

	BR_TRANSACTION_COMPLETE = _IO('r', 6),
	/*
	 * No parameters... always refers to the last transaction requested
	 * (including replies).  Note that this will be sent even for
	 * asynchronous transactions.
	 */

	BR_INCREFS = _IOR('r', 7, struct binder_ptr_cookie),
	BR_ACQUIRE = _IOR('r', 8, struct binder_ptr_cookie),
	BR_RELEASE = _IOR('r', 9, struct binder_ptr_cookie),
	BR_DECREFS = _IOR('r', 10, struct binder_ptr_cookie),
	/*
	 * void *:	ptr to binder
	 * void *: cookie for binder
	 */

	BR_ATTEMPT_ACQUIRE = _IOR('r', 11, struct binder_pri_ptr_cookie),
	/*
	 * not currently supported
	 * int:	priority
	 * void *: ptr to binder
	 * void *: cookie for binder
	 */

	BR_NOOP = _IO('r', 12),
	/*
	 * No parameters.  Do nothing and examine the next command.  It exists
	 * primarily so that we can replace it with a BR_SPAWN_LOOPER command.
	 */

	BR_SPAWN_LOOPER = _IO('r', 13),
	/*
	 * No parameters.  The driver has determined that a process has no
	 * threads waiting to service incomming transactions.  When a process
	 * receives this command, it must spawn a new service thread and
	 * register it via bcENTER_LOOPER.
	 */

	BR_FINISHED = _IO('r', 14),
	/*
	 * not currently supported
	 * stop threadpool thread
	 */

	BR_DEAD_BINDER = _IOR('r', 15, void *),
	/*
	 * void *: cookie
	 */
	BR_CLEAR_DEATH_NOTIFICATION_DONE = _IOR('r', 16, void *),
	/*
	 * void *: cookie
	 */

	BR_FAILED_REPLY = _IO('r', 17),
	/*
	 * The the last transaction (either a bcTRANSACTION or
	 * a bcATTEMPT_ACQUIRE) failed (e.g. out of memory).  No parameters.
	 */
};

enum BinderDriverCommandProtocol {
	BC_TRANSACTION = _IOW('c', 0, struct binder_transaction_data),
	BC_REPLY = _IOW('c', 1, struct binder_transaction_data),
	/*
	 * binder_transaction_data: the sent command.
	 */

	BC_ACQUIRE_RESULT = _IOW('c', 2, int),
	/*
	 * not currently supported
	 * int:  0 if the last BR_ATTEMPT_ACQUIRE was not successful.
	 * Else you have acquired a primary reference on the object.
	 */

	BC_FREE_BUFFER = _IOW('c', 3, int),
	/*
	 * void *: ptr to transaction data received on a read
	 */

	BC_INCREFS = _IOW('c', 4, int),
	BC_ACQUIRE = _IOW('c', 5, int),
	BC_RELEASE = _IOW('c', 6, int),
	BC_DECREFS = _IOW('c', 7, int),
	/*
	 * int:	descriptor
	 */

	BC_INCREFS_DONE = _IOW('c', 8, struct binder_ptr_cookie),
	BC_ACQUIRE_DONE = _IOW('c', 9, struct binder_ptr_cookie),
	/*
	 * void *: ptr to binder
	 * void *: cookie for binder
	 */

	BC_ATTEMPT_ACQUIRE = _IOW('c', 10, struct binder_pri_desc),
	/*
	 * not currently supported
	 * int: priority
	 * int: descriptor
	 */

	BC_REGISTER_LOOPER = _IO('c', 11),
	/*
	 * No parameters.
	 * Register a spawned looper thread with the device.
	 */

	BC_ENTER_LOOPER = _IO('c', 12),
	BC_EXIT_LOOPER = _IO('c', 13),
	/*
	 * No parameters.
	 * These two commands are sent as an application-level thread
	 * enters and exits the binder loop, respectively.  They are
	 * used so the binder can have an accurate count of the number
	 * of looping threads it has available.
	 */

	BC_REQUEST_DEATH_NOTIFICATION = _IOW('c', 14, struct binder_ptr_cookie),
	/*
	 * void *: ptr to binder
	 * void *: cookie
	 */

	BC_CLEAR_DEATH_NOTIFICATION = _IOW('c', 15, struct binder_ptr_cookie),
	/*
	 * void *: ptr to binder
	 * void *: cookie
	 */

	BC_DEAD_BINDER_DONE = _IOW('c', 16, void *),
	/*
	 * void *: cookie
	 */
};



/***********************************************************************************/
/********************** DRM **********************************************/
/***********************************************************************************/

#include "config.h"


#ifdef USE_DRM

#include <drm/drm.h>
#ifdef USE_DRM_EXYNOS
#include <drm/exynos_drm.h>
#endif
#include <drm/i810_drm.h>
#include <drm/i915_drm.h>
#include <drm/mga_drm.h>
#include <drm/nouveau_drm.h>
#include <drm/r128_drm.h>
#include <drm/radeon_drm.h>
#include <drm/savage_drm.h>

/* Copy-paste relevant stuff from header, as it is buggy. */
/* #include <drm/sis_drm.h> */

/* SiS specific ioctls */
#define NOT_USED_0_3
#define DRM_SIS_FB_ALLOC	0x04
#define DRM_SIS_FB_FREE	        0x05
#define NOT_USED_6_12
#define DRM_SIS_AGP_INIT	0x13
#define DRM_SIS_AGP_ALLOC	0x14
#define DRM_SIS_AGP_FREE	0x15
#define DRM_SIS_FB_INIT	        0x16

#define DRM_IOCTL_SIS_FB_ALLOC		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_FB_ALLOC, drm_sis_mem_t)
#define DRM_IOCTL_SIS_FB_FREE		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_FB_FREE, drm_sis_mem_t)
#define DRM_IOCTL_SIS_AGP_INIT		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_AGP_INIT, drm_sis_agp_t)
#define DRM_IOCTL_SIS_AGP_ALLOC		DRM_IOWR(DRM_COMMAND_BASE + DRM_SIS_AGP_ALLOC, drm_sis_mem_t)
#define DRM_IOCTL_SIS_AGP_FREE		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_AGP_FREE, drm_sis_mem_t)
#define DRM_IOCTL_SIS_FB_INIT		DRM_IOW( DRM_COMMAND_BASE + DRM_SIS_FB_INIT, drm_sis_fb_t)
/*
#define DRM_IOCTL_SIS_FLIP		DRM_IOW( 0x48, drm_sis_flip_t)
#define DRM_IOCTL_SIS_FLIP_INIT		DRM_IO(  0x49)
#define DRM_IOCTL_SIS_FLIP_FINAL	DRM_IO(  0x50)
*/

/* deprecated nouveau ioctls */
/*
 * Copyright 2005 Stephane Marchesin.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef DRM_IOCTL_NOUVEAU_GETPARAM
struct drm_nouveau_getparam {
	uint64_t param;
	uint64_t value;
};
#define DRM_IOCTL_NOUVEAU_GETPARAM           DRM_IOWR(DRM_COMMAND_BASE + DRM_NOUVEAU_GETPARAM, struct drm_nouveau_getparam)
#endif

#ifndef DRM_IOCTL_NOUVEAU_SETPARAM
struct drm_nouveau_setparam {
	uint64_t param;
	uint64_t value;
};
#define DRM_IOCTL_NOUVEAU_SETPARAM           DRM_IOWR(DRM_COMMAND_BASE + DRM_NOUVEAU_SETPARAM, struct drm_nouveau_setparam)
#endif

#ifndef DRM_IOCTL_NOUVEAU_CHANNEL_ALLOC
struct drm_nouveau_channel_alloc {
	uint32_t     fb_ctxdma_handle;
	uint32_t     tt_ctxdma_handle;

	int          channel;
	uint32_t     pushbuf_domains;

	/* Notifier memory */
	uint32_t     notifier_handle;

	/* DRM-enforced subchannel assignments */
	struct {
		uint32_t handle;
		uint32_t grclass;
	} subchan[8];
	uint32_t nr_subchan;
};
#define DRM_IOCTL_NOUVEAU_CHANNEL_ALLOC      DRM_IOWR(DRM_COMMAND_BASE + DRM_NOUVEAU_CHANNEL_ALLOC, struct drm_nouveau_channel_alloc)
#endif

#ifndef DRM_IOCTL_NOUVEAU_CHANNEL_FREE
struct drm_nouveau_channel_free {
	int channel;
};
#define DRM_IOCTL_NOUVEAU_CHANNEL_FREE       DRM_IOW (DRM_COMMAND_BASE + DRM_NOUVEAU_CHANNEL_FREE, struct drm_nouveau_channel_free)
#endif

#ifndef DRM_IOCTL_NOUVEAU_GROBJ_ALLOC
struct drm_nouveau_grobj_alloc {
	int      channel;
	uint32_t handle;
	int      class;
};
#define DRM_IOCTL_NOUVEAU_GROBJ_ALLOC        DRM_IOW (DRM_COMMAND_BASE + DRM_NOUVEAU_GROBJ_ALLOC, struct drm_nouveau_grobj_alloc)
#endif

#ifndef DRM_IOCTL_NOUVEAU_NOTIFIEROBJ_ALLOC
struct drm_nouveau_notifierobj_alloc {
	uint32_t channel;
	uint32_t handle;
	uint32_t size;
	uint32_t offset;
};
#define DRM_IOCTL_NOUVEAU_NOTIFIEROBJ_ALLOC  DRM_IOWR(DRM_COMMAND_BASE + DRM_NOUVEAU_NOTIFIEROBJ_ALLOC, struct drm_nouveau_notifierobj_alloc)
#endif

#ifndef DRM_IOCTL_NOUVEAU_GPUOBJ_FREE
struct drm_nouveau_gpuobj_free {
	int      channel;
	uint32_t handle;
};
#define DRM_IOCTL_NOUVEAU_GPUOBJ_FREE        DRM_IOW (DRM_COMMAND_BASE + DRM_NOUVEAU_GPUOBJ_FREE, struct drm_nouveau_gpuobj_free)
#endif

typedef struct {
	int context;
	unsigned long offset;
	unsigned long size;
	unsigned long free;
} drm_sis_mem_t;

typedef struct {
	unsigned long offset, size;
} drm_sis_agp_t;

typedef struct {
	unsigned long offset, size;
} drm_sis_fb_t;

/* header is buggy. */
/* #include <drm/via_drm.h> */

#endif //USE_DRM



/******************************************************************/
/****** MSM_VIDC_ENC **************************************************/
/*****************************************************************/

/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_S_BASE 0x00000000
#define VEN_S_SUCCESS (VEN_S_BASE) 
#define VEN_S_EFAIL (VEN_S_BASE+1) 
#define VEN_S_EFATAL (VEN_S_BASE+2) 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_S_EBADPARAM (VEN_S_BASE+3) 
#define VEN_S_EINVALSTATE (VEN_S_BASE+4)
#define VEN_S_ENOSWRES (VEN_S_BASE+5) 
#define VEN_S_ENOHWRES (VEN_S_BASE+6) 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_S_EBUFFREQ (VEN_S_BASE+7) 
#define VEN_S_EINVALCMD (VEN_S_BASE+8) 
#define VEN_S_ETIMEOUT (VEN_S_BASE+9) 
#define VEN_S_ENOREATMPT (VEN_S_BASE+10)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_S_ENOPREREQ (VEN_S_BASE+11) 
#define VEN_S_ECMDQFULL (VEN_S_BASE+12) 
#define VEN_S_ENOTSUPP (VEN_S_BASE+13) 
#define VEN_S_ENOTIMPL (VEN_S_BASE+14) 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_S_ENOTPMEM (VEN_S_BASE+15) 
#define VEN_S_EFLUSHED (VEN_S_BASE+16) 
#define VEN_S_EINSUFBUF (VEN_S_BASE+17) 
#define VEN_S_ESAMESTATE (VEN_S_BASE+18)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_S_EINVALTRANS (VEN_S_BASE+19)
#define VEN_INTF_VER 1
#define VEN_MSG_INDICATION 0
#define VEN_MSG_INPUT_BUFFER_DONE 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_MSG_OUTPUT_BUFFER_DONE 2
#define VEN_MSG_NEED_OUTPUT_BUFFER 3
#define VEN_MSG_FLUSH_INPUT_DONE 4
#define VEN_MSG_FLUSH_OUPUT_DONE 5
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_MSG_START 6
#define VEN_MSG_STOP 7
#define VEN_MSG_PAUSE 8
#define VEN_MSG_RESUME 9
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_MSG_STOP_READING_MSG 10
#define VEN_BUFFLAG_EOS 0x00000001
#define VEN_BUFFLAG_ENDOFFRAME 0x00000010
#define VEN_BUFFLAG_SYNCFRAME 0x00000020
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_BUFFLAG_EXTRADATA 0x00000040
#define VEN_BUFFLAG_CODECCONFIG 0x00000080
#define VEN_EXTRADATA_NONE 0x001
#define VEN_EXTRADATA_QCOMFILLER 0x002
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_EXTRADATA_SLICEINFO 0x100
#define VEN_FRAME_TYPE_I 1 
#define VEN_FRAME_TYPE_P 2 
#define VEN_FRAME_TYPE_B 3 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_CODEC_MPEG4 1 
#define VEN_CODEC_H264 2 
#define VEN_CODEC_H263 3 
#define VEN_PROFILE_MPEG4_SP 1 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_PROFILE_MPEG4_ASP 2 
#define VEN_PROFILE_H264_BASELINE 3 
#define VEN_PROFILE_H264_MAIN 4 
#define VEN_PROFILE_H264_HIGH 5 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_PROFILE_H263_BASELINE 6 
#define VEN_LEVEL_MPEG4_0 0x1 
#define VEN_LEVEL_MPEG4_1 0x2 
#define VEN_LEVEL_MPEG4_2 0x3 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_LEVEL_MPEG4_3 0x4 
#define VEN_LEVEL_MPEG4_4 0x5 
#define VEN_LEVEL_MPEG4_5 0x6 
#define VEN_LEVEL_MPEG4_3b 0x7 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_LEVEL_MPEG4_6 0x8 
#define VEN_LEVEL_H264_1 0x9 
#define VEN_LEVEL_H264_1b 0xA 
#define VEN_LEVEL_H264_1p1 0xB 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_LEVEL_H264_1p2 0xC 
#define VEN_LEVEL_H264_1p3 0xD 
#define VEN_LEVEL_H264_2 0xE 
#define VEN_LEVEL_H264_2p1 0xF 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_LEVEL_H264_2p2 0x10 
#define VEN_LEVEL_H264_3 0x11 
#define VEN_LEVEL_H264_3p1 0x12 
#define VEN_LEVEL_H264_3p2 0x13 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_LEVEL_H264_4 0x14 
#define VEN_LEVEL_H263_10 0x15 
#define VEN_LEVEL_H263_20 0x16 
#define VEN_LEVEL_H263_30 0x17 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_LEVEL_H263_40 0x18 
#define VEN_LEVEL_H263_45 0x19 
#define VEN_LEVEL_H263_50 0x1A 
#define VEN_LEVEL_H263_60 0x1B 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_LEVEL_H263_70 0x1C 
#define VEN_ENTROPY_MODEL_CAVLC 1
#define VEN_ENTROPY_MODEL_CABAC 2
#define VEN_CABAC_MODEL_0 1 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_CABAC_MODEL_1 2 
#define VEN_CABAC_MODEL_2 3 
#define VEN_DB_DISABLE 1 
#define VEN_DB_ALL_BLKG_BNDRY 2 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_DB_SKIP_SLICE_BNDRY 3 
#define VEN_MSLICE_OFF 1
#define VEN_MSLICE_CNT_MB 2  
#define VEN_MSLICE_CNT_BYTE 3  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_MSLICE_GOB 4  
#define VEN_RC_OFF 1
#define VEN_RC_VBR_VFR 2
#define VEN_RC_VBR_CFR 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_RC_CBR_VFR 4
#define VEN_RC_CBR_CFR 5
#define VEN_FLUSH_INPUT 1
#define VEN_FLUSH_OUTPUT 2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_FLUSH_ALL 3
#define VEN_INPUTFMT_NV12 1 
#define VEN_INPUTFMT_NV21 2 
#define VEN_INPUTFMT_NV12_16M2KA 3 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_ROTATION_0 1 
#define VEN_ROTATION_90 2 
#define VEN_ROTATION_180 3 
#define VEN_ROTATION_270 4 
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_TIMEOUT_INFINITE 0xffffffff
#define VEN_IR_OFF 1
#define VEN_IR_CYCLIC 2
#define VEN_IR_RANDOM 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTLBASE_NENC 0x800
#define VEN_IOCTLBASE_ENC 0x850
struct venc_ioctl_msg{
  void __user *in;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  void __user *out;
};


#define VEN_IOCTL_SET_INTF_VERSION   _IOW(VEN_IOCTLBASE_NENC, 0, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_READ_NEXT_MSG   _IOWR(VEN_IOCTLBASE_NENC, 1, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_CMD_STOP_READ_MSG _IO(VEN_IOCTLBASE_NENC, 2)
#define VEN_IOCTL_SET_INPUT_BUFFER_REQ   _IOW(VEN_IOCTLBASE_NENC, 3, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_INPUT_BUFFER_REQ   _IOR(VEN_IOCTLBASE_NENC, 4, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_ALLOC_INPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 5, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_INPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 6, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_FREE_INPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 7, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_OUTPUT_BUFFER_REQ   _IOW(VEN_IOCTLBASE_NENC, 8, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_OUTPUT_BUFFER_REQ   _IOR(VEN_IOCTLBASE_NENC, 9, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_CMD_ALLOC_OUTPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 10, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_OUTPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 11, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_FREE_OUTPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 12, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_START _IO(VEN_IOCTLBASE_NENC, 13)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_CMD_ENCODE_FRAME   _IOW(VEN_IOCTLBASE_NENC, 14, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_FILL_OUTPUT_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 15, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_FLUSH   _IOW(VEN_IOCTLBASE_NENC, 16, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_PAUSE _IO(VEN_IOCTLBASE_NENC, 17)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_CMD_RESUME _IO(VEN_IOCTLBASE_NENC, 18)
#define VEN_IOCTL_CMD_STOP _IO(VEN_IOCTLBASE_NENC, 19)
#define VEN_IOCTL_SET_RECON_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 20, struct venc_ioctl_msg)
#define VEN_IOCTL_FREE_RECON_BUFFER   _IOW(VEN_IOCTLBASE_NENC, 21, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_GET_RECON_BUFFER_SIZE   _IOW(VEN_IOCTLBASE_NENC, 22, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_BASE_CFG   _IOW(VEN_IOCTLBASE_ENC, 1, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_BASE_CFG   _IOR(VEN_IOCTLBASE_ENC, 2, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_LIVE_MODE   _IOW(VEN_IOCTLBASE_ENC, 3, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_GET_LIVE_MODE   _IOR(VEN_IOCTLBASE_ENC, 4, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_CODEC_PROFILE   _IOW(VEN_IOCTLBASE_ENC, 5, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_CODEC_PROFILE   _IOR(VEN_IOCTLBASE_ENC, 6, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_PROFILE_LEVEL   _IOW(VEN_IOCTLBASE_ENC, 7, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_GET_PROFILE_LEVEL   _IOR(VEN_IOCTLBASE_ENC, 8, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_SHORT_HDR   _IOW(VEN_IOCTLBASE_ENC, 9, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_SHORT_HDR   _IOR(VEN_IOCTLBASE_ENC, 10, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_SESSION_QP   _IOW(VEN_IOCTLBASE_ENC, 11, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_GET_SESSION_QP   _IOR(VEN_IOCTLBASE_ENC, 12, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_INTRA_PERIOD   _IOW(VEN_IOCTLBASE_ENC, 13, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_INTRA_PERIOD   _IOR(VEN_IOCTLBASE_ENC, 14, struct venc_ioctl_msg)
#define VEN_IOCTL_CMD_REQUEST_IFRAME _IO(VEN_IOCTLBASE_ENC, 15)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_GET_CAPABILITY   _IOR(VEN_IOCTLBASE_ENC, 16, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_SEQUENCE_HDR   _IOR(VEN_IOCTLBASE_ENC, 17, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_ENTROPY_CFG   _IOW(VEN_IOCTLBASE_ENC, 18, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_ENTROPY_CFG   _IOR(VEN_IOCTLBASE_ENC, 19, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_DEBLOCKING_CFG   _IOW(VEN_IOCTLBASE_ENC, 20, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_DEBLOCKING_CFG   _IOR(VEN_IOCTLBASE_ENC, 21, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_INTRA_REFRESH   _IOW(VEN_IOCTLBASE_ENC, 22, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_INTRA_REFRESH   _IOR(VEN_IOCTLBASE_ENC, 23, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_MULTI_SLICE_CFG   _IOW(VEN_IOCTLBASE_ENC, 24, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_MULTI_SLICE_CFG   _IOR(VEN_IOCTLBASE_ENC, 25, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_RATE_CTRL_CFG   _IOW(VEN_IOCTLBASE_ENC, 26, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_RATE_CTRL_CFG   _IOR(VEN_IOCTLBASE_ENC, 27, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_VOP_TIMING_CFG   _IOW(VEN_IOCTLBASE_ENC, 28, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_VOP_TIMING_CFG   _IOR(VEN_IOCTLBASE_ENC, 29, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_FRAME_RATE   _IOW(VEN_IOCTLBASE_ENC, 30, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_FRAME_RATE   _IOR(VEN_IOCTLBASE_ENC, 31, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_TARGET_BITRATE   _IOW(VEN_IOCTLBASE_ENC, 32, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_TARGET_BITRATE   _IOR(VEN_IOCTLBASE_ENC, 33, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_ROTATION   _IOW(VEN_IOCTLBASE_ENC, 34, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_ROTATION   _IOR(VEN_IOCTLBASE_ENC, 35, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_HEC   _IOW(VEN_IOCTLBASE_ENC, 36, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_HEC   _IOR(VEN_IOCTLBASE_ENC, 37, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_DATA_PARTITION   _IOW(VEN_IOCTLBASE_ENC, 38, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_DATA_PARTITION   _IOR(VEN_IOCTLBASE_ENC, 39, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_RVLC   _IOW(VEN_IOCTLBASE_ENC, 40, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_RVLC   _IOR(VEN_IOCTLBASE_ENC, 41, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_AC_PREDICTION   _IOW(VEN_IOCTLBASE_ENC, 42, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_AC_PREDICTION   _IOR(VEN_IOCTLBASE_ENC, 43, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_QP_RANGE   _IOW(VEN_IOCTLBASE_ENC, 44, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_QP_RANGE   _IOR(VEN_IOCTLBASE_ENC, 45, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_NUMBER_INSTANCES   _IOR(VEN_IOCTLBASE_ENC, 46, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_METABUFFER_MODE   _IOW(VEN_IOCTLBASE_ENC, 47, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VEN_IOCTL_SET_EXTRADATA   _IOW(VEN_IOCTLBASE_ENC, 48, struct venc_ioctl_msg)
#define VEN_IOCTL_GET_EXTRADATA   _IOR(VEN_IOCTLBASE_ENC, 49, struct venc_ioctl_msg)
#define VEN_IOCTL_SET_SLICE_DELIVERY_MODE   _IO(VEN_IOCTLBASE_ENC, 50)
#define VEN_IOCTL_SET_SPS_PPS_FOR_IDR   _IOW(VEN_IOCTLBASE_ENC, 51, struct venc_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct venc_switch{
    unsigned char status;
  };
struct venc_allocatorproperty{
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long mincount;
  unsigned long maxcount;
  unsigned long actualcount;
  unsigned long datasize;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long suffixsize;
  unsigned long alignment;
  unsigned long bufpoolid;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct venc_bufferpayload{
  unsigned char *pbuffer;
  size_t sz;
  int fd;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned int offset;
  unsigned int maped_size;
  unsigned long filled_len;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct venc_buffer{
  unsigned char *ptrbuffer;
  unsigned long sz;
  unsigned long len;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long offset;
  long long timestamp;
  unsigned long flags;
  void *clientdata;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct venc_basecfg{
  unsigned long input_width;
  unsigned long input_height;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long dvs_width;
  unsigned long dvs_height;
  unsigned long codectype;
  unsigned long fps_num;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long fps_den;
  unsigned long targetbitrate;
  unsigned long inputformat;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct venc_profile{
  unsigned long profile;
};
struct ven_profilelevel{
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long level;
};
struct venc_sessionqp{
  unsigned long iframeqp;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long pframqp;
};
struct venc_qprange{
  unsigned long maxqp;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long minqp;
};
struct venc_intraperiod{
  unsigned long num_pframes;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long num_bframes;
};
struct venc_seqheader{
  unsigned char *hdrbufptr;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long bufsize;
  unsigned long hdrlen;
};
struct venc_capability{
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long codec_types;
  unsigned long maxframe_width;
  unsigned long maxframe_height;
  unsigned long maxtarget_bitrate;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long maxframe_rate;
  unsigned long input_formats;
  unsigned char dvs;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct venc_entropycfg{
  unsigned longentropysel;
  unsigned long cabacmodel;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct venc_dbcfg{
  unsigned long db_mode;
  unsigned long slicealpha_offset;
  unsigned long slicebeta_offset;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct venc_intrarefresh{
  unsigned long irmode;
  unsigned long mbcount;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct venc_multiclicecfg{
  unsigned long mslice_mode;
  unsigned long mslice_size;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct venc_bufferflush{
  unsigned long flush_mode;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct venc_ratectrlcfg{
  unsigned long rcmode;
};
struct venc_voptimingcfg{
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long voptime_resolution;
};
struct venc_framerate{
  unsigned long fps_denominator;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long fps_numerator;
};
struct venc_targetbitrate{
  unsigned long target_bitrate;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct venc_rotation{
  unsigned long rotation;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct venc_timeout{
  unsigned long millisec;
};
struct venc_headerextension{
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long header_extension;
};
struct venc_msg{
  unsigned long statuscode;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long msgcode;
  struct venc_buffer buf;
  unsigned long msgdata_size;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct venc_recon_addr{
  unsigned char *pbuffer;
  unsigned long buffer_size;
  unsigned long pmem_fd;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  unsigned long offset;
};
struct venc_recon_buff_size{
  int width;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int height;
  int size;
  int alignment;
};



/********************************** KGSL ****************************************/
/********************************************************************************/

#define KGSL_VERSION_MAJOR        3
#define KGSL_VERSION_MINOR        14

/*context flags */
#define KGSL_CONTEXT_SAVE_GMEM		  0x00000001
#define KGSL_CONTEXT_NO_GMEM_ALLOC	  0x00000002
#define KGSL_CONTEXT_SUBMIT_IB_LIST	  0x00000004
#define KGSL_CONTEXT_CTX_SWITCH		  0x00000008
#define KGSL_CONTEXT_PREAMBLE		  0x00000010
#define KGSL_CONTEXT_TRASH_STATE	  0x00000020
#define KGSL_CONTEXT_PER_CONTEXT_TS	  0x00000040
#define KGSL_CONTEXT_USER_GENERATED_TS	  0x00000080
#define KGSL_CONTEXT_END_OF_FRAME         0x00000100
#define KGSL_CONTEXT_NO_FAULT_TOLERANCE	  0x00000200
/* bits [12:15] are reserved for future use */
#define KGSL_CONTEXT_TYPE_MASK            0x01F00000
#define KGSL_CONTEXT_TYPE_SHIFT           20

#define KGSL_CONTEXT_TYPE_ANY		  0
#define KGSL_CONTEXT_TYPE_GL		  1
#define KGSL_CONTEXT_TYPE_CL		  2
#define KGSL_CONTEXT_TYPE_C2D		  3
#define KGSL_CONTEXT_TYPE_RS		  4

#define KGSL_CONTEXT_INVALID 0xffffffff

/* --- Memory allocation flags --- */

/* General allocation hints */
#define KGSL_MEMFLAGS_GPUREADONLY 0x01000000
#define KGSL_MEMFLAGS_USE_CPU_MAP 0x10000000

/* Memory caching hints */
#define KGSL_CACHEMODE_MASK 0x0C000000
#define KGSL_CACHEMODE_SHIFT 26

#define KGSL_CACHEMODE_WRITECOMBINE 0
#define KGSL_CACHEMODE_UNCACHED 1
#define KGSL_CACHEMODE_WRITETHROUGH 2
#define KGSL_CACHEMODE_WRITEBACK 3

/* Memory types for which allocations are made */
#define KGSL_MEMTYPE_MASK		0x0000FF00
#define KGSL_MEMTYPE_SHIFT		8

#define KGSL_MEMTYPE_OBJECTANY			0
#define KGSL_MEMTYPE_FRAMEBUFFER		1
#define KGSL_MEMTYPE_RENDERBUFFER		2
#define KGSL_MEMTYPE_ARRAYBUFFER		3
#define KGSL_MEMTYPE_ELEMENTARRAYBUFFER		4
#define KGSL_MEMTYPE_VERTEXARRAYBUFFER		5
#define KGSL_MEMTYPE_TEXTURE			6
#define KGSL_MEMTYPE_SURFACE			7
#define KGSL_MEMTYPE_EGL_SURFACE		8
#define KGSL_MEMTYPE_GL				9
#define KGSL_MEMTYPE_CL				10
#define KGSL_MEMTYPE_CL_BUFFER_MAP		11
#define KGSL_MEMTYPE_CL_BUFFER_NOMAP		12
#define KGSL_MEMTYPE_CL_IMAGE_MAP		13
#define KGSL_MEMTYPE_CL_IMAGE_NOMAP		14
#define KGSL_MEMTYPE_CL_KERNEL_STACK		15
#define KGSL_MEMTYPE_COMMAND			16
#define KGSL_MEMTYPE_2D				17
#define KGSL_MEMTYPE_EGL_IMAGE			18
#define KGSL_MEMTYPE_EGL_SHADOW			19
#define KGSL_MEMTYPE_MULTISAMPLE		20
#define KGSL_MEMTYPE_KERNEL			255

/*
 * Alignment hint, passed as the power of 2 exponent.
 * i.e 4k (2^12) would be 12, 64k (2^16)would be 16.
 */
#define KGSL_MEMALIGN_MASK		0x00FF0000
#define KGSL_MEMALIGN_SHIFT		16

/* --- generic KGSL flag values --- */

#define KGSL_FLAGS_NORMALMODE  0x00000000
#define KGSL_FLAGS_SAFEMODE    0x00000001
#define KGSL_FLAGS_INITIALIZED0 0x00000002
#define KGSL_FLAGS_INITIALIZED 0x00000004
#define KGSL_FLAGS_STARTED     0x00000008
#define KGSL_FLAGS_ACTIVE      0x00000010
#define KGSL_FLAGS_RESERVED0   0x00000020
#define KGSL_FLAGS_RESERVED1   0x00000040
#define KGSL_FLAGS_RESERVED2   0x00000080
#define KGSL_FLAGS_SOFT_RESET  0x00000100
#define KGSL_FLAGS_PER_CONTEXT_TIMESTAMPS 0x00000200

/* Clock flags to show which clocks should be controled by a given platform */
#define KGSL_CLK_SRC	0x00000001
#define KGSL_CLK_CORE	0x00000002
#define KGSL_CLK_IFACE	0x00000004
#define KGSL_CLK_MEM	0x00000008
#define KGSL_CLK_MEM_IFACE 0x00000010
#define KGSL_CLK_AXI	0x00000020

/* Server Side Sync Timeout in milliseconds */
#define KGSL_SYNCOBJ_SERVER_TIMEOUT 2000

/*
 * Reset status values for context
 */
enum kgsl_ctx_reset_stat {
	KGSL_CTX_STAT_NO_ERROR				= 0x00000000,
	KGSL_CTX_STAT_GUILTY_CONTEXT_RESET_EXT		= 0x00000001,
	KGSL_CTX_STAT_INNOCENT_CONTEXT_RESET_EXT	= 0x00000002,
	KGSL_CTX_STAT_UNKNOWN_CONTEXT_RESET_EXT		= 0x00000003
};

#define KGSL_CONVERT_TO_MBPS(val) \
	(val*1000*1000U)

/* device id */
enum kgsl_deviceid {
	KGSL_DEVICE_3D0		= 0x00000000,
	KGSL_DEVICE_2D0		= 0x00000001,
	KGSL_DEVICE_2D1		= 0x00000002,
	KGSL_DEVICE_MAX		= 0x00000003
};

enum kgsl_user_mem_type {
	KGSL_USER_MEM_TYPE_PMEM		= 0x00000000,
	KGSL_USER_MEM_TYPE_ASHMEM	= 0x00000001,
	KGSL_USER_MEM_TYPE_ADDR		= 0x00000002,
	KGSL_USER_MEM_TYPE_ION		= 0x00000003,
	KGSL_USER_MEM_TYPE_MAX		= 0x00000004,
};

struct kgsl_devinfo {

	unsigned int device_id;
	/* chip revision id
	* coreid:8 majorrev:8 minorrev:8 patch:8
	*/
	unsigned int chip_id;
	unsigned int mmu_enabled;
	unsigned int gmem_gpubaseaddr;
	/*
	* This field contains the adreno revision
	* number 200, 205, 220, etc...
	*/
	unsigned int gpu_id;
	unsigned int gmem_sizebytes;
};

/* this structure defines the region of memory that can be mmap()ed from this
   driver. The timestamp fields are volatile because they are written by the
   GPU
*/
struct kgsl_devmemstore {
	volatile unsigned int soptimestamp;
	unsigned int sbz;
	volatile unsigned int eoptimestamp;
	unsigned int sbz2;
	volatile unsigned int ts_cmp_enable;
	unsigned int sbz3;
	volatile unsigned int ref_wait_ts;
	unsigned int sbz4;
	unsigned int current_context;
	unsigned int sbz5;
};

#define KGSL_MEMSTORE_OFFSET(ctxt_id, field) \
	((ctxt_id)*sizeof(struct kgsl_devmemstore) + \
	 offsetof(struct kgsl_devmemstore, field))

/* timestamp id*/
enum kgsl_timestamp_type {
	KGSL_TIMESTAMP_CONSUMED = 0x00000001, /* start-of-pipeline timestamp */
	KGSL_TIMESTAMP_RETIRED  = 0x00000002, /* end-of-pipeline timestamp*/
	KGSL_TIMESTAMP_QUEUED   = 0x00000003,
};

/* property types - used with kgsl_device_getproperty */
enum kgsl_property_type {
	KGSL_PROP_DEVICE_INFO     = 0x00000001,
	KGSL_PROP_DEVICE_SHADOW   = 0x00000002,
	KGSL_PROP_DEVICE_POWER    = 0x00000003,
	KGSL_PROP_SHMEM           = 0x00000004,
	KGSL_PROP_SHMEM_APERTURES = 0x00000005,
	KGSL_PROP_MMU_ENABLE 	  = 0x00000006,
	KGSL_PROP_INTERRUPT_WAITS = 0x00000007,
	KGSL_PROP_VERSION         = 0x00000008,
	KGSL_PROP_GPU_RESET_STAT  = 0x00000009,
	KGSL_PROP_PWRCTRL         = 0x0000000E,
};

struct kgsl_shadowprop {
	unsigned int gpuaddr;
	unsigned int size;
	unsigned int flags; /* contains KGSL_FLAGS_ values */
};

struct kgsl_version {
	unsigned int drv_major;
	unsigned int drv_minor;
	unsigned int dev_major;
	unsigned int dev_minor;
};

/* Performance counter groups */

#define KGSL_PERFCOUNTER_GROUP_CP 0x0
#define KGSL_PERFCOUNTER_GROUP_RBBM 0x1
#define KGSL_PERFCOUNTER_GROUP_PC 0x2
#define KGSL_PERFCOUNTER_GROUP_VFD 0x3
#define KGSL_PERFCOUNTER_GROUP_HLSQ 0x4
#define KGSL_PERFCOUNTER_GROUP_VPC 0x5
#define KGSL_PERFCOUNTER_GROUP_TSE 0x6
#define KGSL_PERFCOUNTER_GROUP_RAS 0x7
#define KGSL_PERFCOUNTER_GROUP_UCHE 0x8
#define KGSL_PERFCOUNTER_GROUP_TP 0x9
#define KGSL_PERFCOUNTER_GROUP_SP 0xA
#define KGSL_PERFCOUNTER_GROUP_RB 0xB
#define KGSL_PERFCOUNTER_GROUP_PWR 0xC
#define KGSL_PERFCOUNTER_GROUP_VBIF 0xD
#define KGSL_PERFCOUNTER_GROUP_VBIF_PWR 0xE

#define KGSL_PERFCOUNTER_NOT_USED 0xFFFFFFFF

/* structure holds list of ibs */
struct kgsl_ibdesc {
	unsigned int gpuaddr;
	void *hostptr;
	unsigned int sizedwords;
	unsigned int ctrl;
};

/* ioctls */
#define KGSL_IOC_TYPE 0x09

/* get misc info about the GPU
   type should be a value from enum kgsl_property_type
   value points to a structure that varies based on type
   sizebytes is sizeof() that structure
   for KGSL_PROP_DEVICE_INFO, use struct kgsl_devinfo
   this structure contaings hardware versioning info.
   for KGSL_PROP_DEVICE_SHADOW, use struct kgsl_shadowprop
   this is used to find mmap() offset and sizes for mapping
   struct kgsl_memstore into userspace.
*/
struct kgsl_device_getproperty {
	unsigned int type;
	void  *value;
	unsigned int sizebytes;
};

#define IOCTL_KGSL_DEVICE_GETPROPERTY \
	_IOWR(KGSL_IOC_TYPE, 0x2, struct kgsl_device_getproperty)

/* IOCTL_KGSL_DEVICE_READ (0x3) - removed 03/2012
 */

/* block until the GPU has executed past a given timestamp
 * timeout is in milliseconds.
 */
struct kgsl_device_waittimestamp {
	unsigned int timestamp;
	unsigned int timeout;
};

#define IOCTL_KGSL_DEVICE_WAITTIMESTAMP \
	_IOW(KGSL_IOC_TYPE, 0x6, struct kgsl_device_waittimestamp)

struct kgsl_device_waittimestamp_ctxtid {
	unsigned int context_id;
	unsigned int timestamp;
	unsigned int timeout;
};

#define IOCTL_KGSL_DEVICE_WAITTIMESTAMP_CTXTID \
	_IOW(KGSL_IOC_TYPE, 0x7, struct kgsl_device_waittimestamp_ctxtid)

/* issue indirect commands to the GPU.
 * drawctxt_id must have been created with IOCTL_KGSL_DRAWCTXT_CREATE
 * ibaddr and sizedwords must specify a subset of a buffer created
 * with IOCTL_KGSL_SHAREDMEM_FROM_PMEM
 * flags may be a mask of KGSL_CONTEXT_ values
 * timestamp is a returned counter value which can be passed to
 * other ioctls to determine when the commands have been executed by
 * the GPU.
 */
struct kgsl_ringbuffer_issueibcmds {
	unsigned int drawctxt_id;
	void *ibdesc_addr;
	unsigned int numibs;
	unsigned int timestamp; /*output param */
	unsigned int flags;
};

#define IOCTL_KGSL_RINGBUFFER_ISSUEIBCMDS \
	_IOWR(KGSL_IOC_TYPE, 0x10, struct kgsl_ringbuffer_issueibcmds)

/* read the most recently executed timestamp value
 * type should be a value from enum kgsl_timestamp_type
 */
struct kgsl_cmdstream_readtimestamp {
	unsigned int type;
	unsigned int timestamp; /*output param */
};

#define IOCTL_KGSL_CMDSTREAM_READTIMESTAMP_OLD \
	_IOR(KGSL_IOC_TYPE, 0x11, struct kgsl_cmdstream_readtimestamp)

#define IOCTL_KGSL_CMDSTREAM_READTIMESTAMP \
	_IOWR(KGSL_IOC_TYPE, 0x11, struct kgsl_cmdstream_readtimestamp)

/* free memory when the GPU reaches a given timestamp.
 * gpuaddr specify a memory region created by a
 * IOCTL_KGSL_SHAREDMEM_FROM_PMEM call
 * type should be a value from enum kgsl_timestamp_type
 */
struct kgsl_cmdstream_freememontimestamp {
	void *gpuaddr;
	unsigned int type;
	unsigned int timestamp;
};

#define IOCTL_KGSL_CMDSTREAM_FREEMEMONTIMESTAMP \
	_IOW(KGSL_IOC_TYPE, 0x12, struct kgsl_cmdstream_freememontimestamp)

/* Previous versions of this header had incorrectly defined
   IOCTL_KGSL_CMDSTREAM_FREEMEMONTIMESTAMP as a read-only ioctl instead
   of a write only ioctl.  To ensure binary compatability, the following
   #define will be used to intercept the incorrect ioctl
*/

#define IOCTL_KGSL_CMDSTREAM_FREEMEMONTIMESTAMP_OLD \
	_IOR(KGSL_IOC_TYPE, 0x12, struct kgsl_cmdstream_freememontimestamp)

/* create a draw context, which is used to preserve GPU state.
 * The flags field may contain a mask KGSL_CONTEXT_*  values
 */
struct kgsl_drawctxt_create {
	unsigned int flags;
	unsigned int drawctxt_id; /*output param */
};

#define IOCTL_KGSL_DRAWCTXT_CREATE \
	_IOWR(KGSL_IOC_TYPE, 0x13, struct kgsl_drawctxt_create)

/* destroy a draw context */
struct kgsl_drawctxt_destroy {
	unsigned int drawctxt_id;
};

#define IOCTL_KGSL_DRAWCTXT_DESTROY \
	_IOW(KGSL_IOC_TYPE, 0x14, struct kgsl_drawctxt_destroy)

/* add a block of pmem, fb, ashmem or user allocated address
 * into the GPU address space */
struct kgsl_map_user_mem {
	int fd;
	void *gpuaddr;   /*output param */
	unsigned int len;
	unsigned int offset;
	void *hostptr;   /*input param */
	enum kgsl_user_mem_type memtype;
	unsigned int flags;
};

#define IOCTL_KGSL_MAP_USER_MEM \
	_IOWR(KGSL_IOC_TYPE, 0x15, struct kgsl_map_user_mem)

struct kgsl_cmdstream_readtimestamp_ctxtid {
	unsigned int context_id;
	unsigned int type;
	unsigned int timestamp; /*output param */
};

#define IOCTL_KGSL_CMDSTREAM_READTIMESTAMP_CTXTID \
	_IOWR(KGSL_IOC_TYPE, 0x16, struct kgsl_cmdstream_readtimestamp_ctxtid)

struct kgsl_cmdstream_freememontimestamp_ctxtid {
	unsigned int context_id;
	void *gpuaddr;
	unsigned int type;
	unsigned int timestamp;
};

#define IOCTL_KGSL_CMDSTREAM_FREEMEMONTIMESTAMP_CTXTID \
	_IOW(KGSL_IOC_TYPE, 0x17, \
	struct kgsl_cmdstream_freememontimestamp_ctxtid)

/* add a block of pmem or fb into the GPU address space */
struct kgsl_sharedmem_from_pmem {
	int pmem_fd;
	void *gpuaddr;	/*output param */
	unsigned int len;
	unsigned int offset;
};

#define IOCTL_KGSL_SHAREDMEM_FROM_PMEM \
	_IOWR(KGSL_IOC_TYPE, 0x20, struct kgsl_sharedmem_from_pmem)

/* remove memory from the GPU's address space */
struct kgsl_sharedmem_free {
	void *gpuaddr;
};

#define IOCTL_KGSL_SHAREDMEM_FREE \
	_IOW(KGSL_IOC_TYPE, 0x21, struct kgsl_sharedmem_free)

struct kgsl_cff_user_event {
	unsigned char cff_opcode;
	unsigned int op1;
	unsigned int op2;
	unsigned int op3;
	unsigned int op4;
	unsigned int op5;
	unsigned int __pad[2];
};

#define IOCTL_KGSL_CFF_USER_EVENT \
	_IOW(KGSL_IOC_TYPE, 0x31, struct kgsl_cff_user_event)

struct kgsl_gmem_desc {
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	unsigned int pitch;
};

struct kgsl_buffer_desc {
	void *hostptr;
	void *gpuaddr;
	int   size;
	unsigned int format;
	unsigned int pitch;
	unsigned int enabled;
};

struct kgsl_bind_gmem_shadow {
	unsigned int drawctxt_id;
	struct kgsl_gmem_desc gmem_desc;
	unsigned int shadow_x;
	unsigned int shadow_y;
	struct kgsl_buffer_desc shadow_buffer;
	unsigned int buffer_id;
};

#define IOCTL_KGSL_DRAWCTXT_BIND_GMEM_SHADOW \
    _IOW(KGSL_IOC_TYPE, 0x22, struct kgsl_bind_gmem_shadow)

/* add a block of memory into the GPU address space */

/*
 * IOCTL_KGSL_SHAREDMEM_FROM_VMALLOC deprecated 09/2012
 * use IOCTL_KGSL_GPUMEM_ALLOC instead
 */

struct kgsl_sharedmem_from_vmalloc {
	void *gpuaddr;	/*output param */
	void *hostptr;
	unsigned int flags;
};

#define IOCTL_KGSL_SHAREDMEM_FROM_VMALLOC \
	_IOWR(KGSL_IOC_TYPE, 0x23, struct kgsl_sharedmem_from_vmalloc)

/*
 * This is being deprecated in favor of IOCTL_KGSL_GPUMEM_CACHE_SYNC which
 * supports both directions (flush and invalidate). This code will still
 * work, but by definition it will do a flush of the cache which might not be
 * what you want to have happen on a buffer following a GPU operation.  It is
 * safer to go with IOCTL_KGSL_GPUMEM_CACHE_SYNC
 */

#define IOCTL_KGSL_SHAREDMEM_FLUSH_CACHE \
	_IOW(KGSL_IOC_TYPE, 0x24, struct kgsl_sharedmem_free)

struct kgsl_drawctxt_set_bin_base_offset {
	unsigned int drawctxt_id;
	unsigned int offset;
};

#define IOCTL_KGSL_DRAWCTXT_SET_BIN_BASE_OFFSET \
	_IOW(KGSL_IOC_TYPE, 0x25, struct kgsl_drawctxt_set_bin_base_offset)

enum kgsl_cmdwindow_type {
	KGSL_CMDWINDOW_MIN     = 0x00000000,
	KGSL_CMDWINDOW_2D      = 0x00000000,
	KGSL_CMDWINDOW_3D      = 0x00000001, /* legacy */
	KGSL_CMDWINDOW_MMU     = 0x00000002,
	KGSL_CMDWINDOW_ARBITER = 0x000000FF,
	KGSL_CMDWINDOW_MAX     = 0x000000FF,
};

/* write to the command window */
struct kgsl_cmdwindow_write {
	enum kgsl_cmdwindow_type target;
	void *addr;
	unsigned int data;
};

#define IOCTL_KGSL_CMDWINDOW_WRITE \
	_IOW(KGSL_IOC_TYPE, 0x2e, struct kgsl_cmdwindow_write)

struct kgsl_gpumem_alloc {
	void *gpuaddr;
	size_t size;
	unsigned int flags;
};

#define IOCTL_KGSL_GPUMEM_ALLOC \
	_IOWR(KGSL_IOC_TYPE, 0x2f, struct kgsl_gpumem_alloc)

struct kgsl_cff_syncmem {
	void *gpuaddr;
	unsigned int len;
	unsigned int __pad[2]; /* For future binary compatibility */
};

#define IOCTL_KGSL_CFF_SYNCMEM \
	_IOW(KGSL_IOC_TYPE, 0x30, struct kgsl_cff_syncmem)

/*
 * A timestamp event allows the user space to register an action following an
 * expired timestamp. Note IOCTL_KGSL_TIMESTAMP_EVENT has been redefined to
 * _IOWR to support fences which need to return a fd for the priv parameter.
 */

struct kgsl_timestamp_event {
	int type;                /* Type of event (see list below) */
	unsigned int timestamp;  /* Timestamp to trigger event on */
	unsigned int context_id; /* Context for the timestamp */
	void *priv;              /* Pointer to the event specific blob */
	size_t len;              /* Size of the event specific blob */
};

#define IOCTL_KGSL_TIMESTAMP_EVENT_OLD \
	_IOW(KGSL_IOC_TYPE, 0x31, struct kgsl_timestamp_event)

/* A genlock timestamp event releases an existing lock on timestamp expire */

#define KGSL_TIMESTAMP_EVENT_GENLOCK 1

struct kgsl_timestamp_event_genlock {
	int handle; /* Handle of the genlock lock to release */
};

/* A fence timestamp event releases an existing lock on timestamp expire */

#define KGSL_TIMESTAMP_EVENT_FENCE 2

struct kgsl_timestamp_event_fence {
	int fence_fd; /* Fence to signal */
};

/*
 * Set a property within the kernel.  Uses the same structure as
 * IOCTL_KGSL_GETPROPERTY
 */

#define IOCTL_KGSL_SETPROPERTY \
	_IOW(KGSL_IOC_TYPE, 0x32, struct kgsl_device_getproperty)

#define IOCTL_KGSL_TIMESTAMP_EVENT \
	_IOWR(KGSL_IOC_TYPE, 0x33, struct kgsl_timestamp_event)

/**
 * struct kgsl_gpumem_alloc_id - argument to IOCTL_KGSL_GPUMEM_ALLOC_ID
 * @id: returned id value for this allocation.
 * @flags: mask of KGSL_MEM* values requested and actual flags on return.
 * @size: requested size of the allocation and actual size on return.
 * @mmapsize: returned size to pass to mmap() which may be larger than 'size'
 * @gpuaddr: returned GPU address for the allocation
 *
 * Allocate memory for access by the GPU. The flags and size fields are echoed
 * back by the kernel, so that the caller can know if the request was
 * adjusted.
 *
 * Supported flags:
 * KGSL_MEMFLAGS_GPUREADONLY: the GPU will be unable to write to the buffer
 * KGSL_MEMTYPE*: usage hint for debugging aid
 * KGSL_MEMALIGN*: alignment hint, may be ignored or adjusted by the kernel.
 * KGSL_MEMFLAGS_USE_CPU_MAP: If set on call and return, the returned GPU
 * address will be 0. Calling mmap() will set the GPU address.
 */
struct kgsl_gpumem_alloc_id {
	unsigned int id;
	unsigned int flags;
	unsigned int size;
	unsigned int mmapsize;
	void  *gpuaddr;
/* private: reserved for future use*/
	unsigned int __pad[2];
};

#define IOCTL_KGSL_GPUMEM_ALLOC_ID \
	_IOWR(KGSL_IOC_TYPE, 0x34, struct kgsl_gpumem_alloc_id)

/**
 * struct kgsl_gpumem_free_id - argument to IOCTL_KGSL_GPUMEM_FREE_ID
 * @id: GPU allocation id to free
 *
 * Free an allocation by id, in case a GPU address has not been assigned or
 * is unknown. Freeing an allocation by id with this ioctl or by GPU address
 * with IOCTL_KGSL_SHAREDMEM_FREE are equivalent.
 */
struct kgsl_gpumem_free_id {
	unsigned int id;
/* private: reserved for future use*/
	unsigned int __pad;
};

#define IOCTL_KGSL_GPUMEM_FREE_ID \
	_IOWR(KGSL_IOC_TYPE, 0x35, struct kgsl_gpumem_free_id)

/**
 * struct kgsl_gpumem_get_info - argument to IOCTL_KGSL_GPUMEM_GET_INFO
 * @gpuaddr: GPU address to query. Also set on return.
 * @id: GPU allocation id to query. Also set on return.
 * @flags: returned mask of KGSL_MEM* values.
 * @size: returned size of the allocation.
 * @mmapsize: returned size to pass mmap(), which may be larger than 'size'
 * @useraddr: returned address of the userspace mapping for this buffer
 *
 * This ioctl allows querying of all user visible attributes of an existing
 * allocation, by either the GPU address or the id returned by a previous
 * call to IOCTL_KGSL_GPUMEM_ALLOC_ID. Legacy allocation ioctls may not
 * return all attributes so this ioctl can be used to look them up if needed.
 *
 */
struct kgsl_gpumem_get_info {
	void *gpuaddr;
	unsigned int id;
	unsigned int flags;
	unsigned int size;
	unsigned int mmapsize;
	void *useraddr;
/* private: reserved for future use*/
	unsigned int __pad[4];
};

#define IOCTL_KGSL_GPUMEM_GET_INFO\
	_IOWR(KGSL_IOC_TYPE, 0x36, struct kgsl_gpumem_get_info)

/**
 * struct kgsl_gpumem_sync_cache - argument to IOCTL_KGSL_GPUMEM_SYNC_CACHE
 * @gpuaddr: GPU address of the buffer to sync.
 * @id: id of the buffer to sync. Either gpuaddr or id is sufficient.
 * @op: a mask of KGSL_GPUMEM_CACHE_* values
 *
 * Sync the L2 cache for memory headed to and from the GPU - this replaces
 * KGSL_SHAREDMEM_FLUSH_CACHE since it can handle cache management for both
 * directions
 *
 */
struct kgsl_gpumem_sync_cache {
	void *gpuaddr;
	unsigned int id;
	unsigned int op;
/* private: reserved for future use*/
	unsigned int __pad[2]; /* For future binary compatibility */
};

#define KGSL_GPUMEM_CACHE_CLEAN (1 << 0)
#define KGSL_GPUMEM_CACHE_TO_GPU KGSL_GPUMEM_CACHE_CLEAN

#define KGSL_GPUMEM_CACHE_INV (1 << 1)
#define KGSL_GPUMEM_CACHE_FROM_GPU KGSL_GPUMEM_CACHE_INV

#define KGSL_GPUMEM_CACHE_FLUSH \
	(KGSL_GPUMEM_CACHE_CLEAN | KGSL_GPUMEM_CACHE_INV)

#define IOCTL_KGSL_GPUMEM_SYNC_CACHE \
	_IOW(KGSL_IOC_TYPE, 0x37, struct kgsl_gpumem_sync_cache)

/**
 * struct kgsl_perfcounter_get - argument to IOCTL_KGSL_PERFCOUNTER_GET
 * @groupid: Performance counter group ID
 * @countable: Countable to select within the group
 * @offset: Return offset of the reserved counter
 *
 * Get an available performance counter from a specified groupid.  The offset
 * of the performance counter will be returned after successfully assigning
 * the countable to the counter for the specified group.  An error will be
 * returned and an offset of 0 if the groupid is invalid or there are no
 * more counters left.  After successfully getting a perfcounter, the user
 * must call kgsl_perfcounter_put(groupid, contable) when finished with
 * the perfcounter to clear up perfcounter resources.
 *
 */
struct kgsl_perfcounter_get {
	unsigned int groupid;
	unsigned int countable;
	unsigned int offset;
/* private: reserved for future use */
	unsigned int __pad[2]; /* For future binary compatibility */
};

#define IOCTL_KGSL_PERFCOUNTER_GET \
	_IOWR(KGSL_IOC_TYPE, 0x38, struct kgsl_perfcounter_get)

/**
 * struct kgsl_perfcounter_put - argument to IOCTL_KGSL_PERFCOUNTER_PUT
 * @groupid: Performance counter group ID
 * @countable: Countable to release within the group
 *
 * Put an allocated performance counter to allow others to have access to the
 * resource that was previously taken.  This is only to be called after
 * successfully getting a performance counter from kgsl_perfcounter_get().
 *
 */
struct kgsl_perfcounter_put {
	unsigned int groupid;
	unsigned int countable;
/* private: reserved for future use */
	unsigned int __pad[2]; /* For future binary compatibility */
};

#define IOCTL_KGSL_PERFCOUNTER_PUT \
	_IOW(KGSL_IOC_TYPE, 0x39, struct kgsl_perfcounter_put)

/**
 * struct kgsl_perfcounter_query - argument to IOCTL_KGSL_PERFCOUNTER_QUERY
 * @groupid: Performance counter group ID
 * @countable: Return active countables array
 * @size: Size of active countables array
 * @max_counters: Return total number counters for the group ID
 *
 * Query the available performance counters given a groupid.  The array
 * *countables is used to return the current active countables in counters.
 * The size of the array is passed in so the kernel will only write at most
 * size or counter->size for the group id.  The total number of available
 * counters for the group ID is returned in max_counters.
 * If the array or size passed in are invalid, then only the maximum number
 * of counters will be returned, no data will be written to *countables.
 * If the groupid is invalid an error code will be returned.
 *
 */
struct kgsl_perfcounter_query {
	unsigned int groupid;
	/* Array to return the current countable for up to size counters */
	unsigned int *countables;
	unsigned int count;
	unsigned int max_counters;
/* private: reserved for future use */
	unsigned int __pad[2]; /* For future binary compatibility */
};

#define IOCTL_KGSL_PERFCOUNTER_QUERY \
	_IOWR(KGSL_IOC_TYPE, 0x3A, struct kgsl_perfcounter_query)

/**
 * struct kgsl_perfcounter_query - argument to IOCTL_KGSL_PERFCOUNTER_QUERY
 * @groupid: Performance counter group IDs
 * @countable: Performance counter countable IDs
 * @value: Return performance counter reads
 * @size: Size of all arrays (groupid/countable pair and return value)
 *
 * Read in the current value of a performance counter given by the groupid
 * and countable.
 *
 */

struct kgsl_perfcounter_read_group {
	unsigned int groupid;
	unsigned int countable;
	uint64_t value;
};

struct kgsl_perfcounter_read {
	struct kgsl_perfcounter_read_group *reads;
	unsigned int count;
/* private: reserved for future use */
	unsigned int __pad[2]; /* For future binary compatibility */
};

#define IOCTL_KGSL_PERFCOUNTER_READ \
	_IOWR(KGSL_IOC_TYPE, 0x3B, struct kgsl_perfcounter_read)

#ifdef __KERNEL__
#ifdef CONFIG_MSM_KGSL_DRM
int kgsl_gem_obj_addr(int drm_fd, int handle, unsigned long *start,
			unsigned long *len);
#else
#define kgsl_gem_obj_addr(...) 0
#endif
#endif



/****************************************/
/*********** MSM_VIDC_DEC ***************/
/****************************************/




/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_S_BASE 0x40000000
#define VDEC_S_SUCCESS (VDEC_S_BASE)
#define VDEC_S_EFAIL (VDEC_S_BASE + 1)
#define VDEC_S_EFATAL (VDEC_S_BASE + 2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_S_EBADPARAM (VDEC_S_BASE + 3)
#define VDEC_S_EINVALSTATE (VDEC_S_BASE + 4)
#define VDEC_S_ENOSWRES (VDEC_S_BASE + 5)
#define VDEC_S_ENOHWRES (VDEC_S_BASE + 6)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_S_EINVALCMD (VDEC_S_BASE + 7)
#define VDEC_S_ETIMEOUT (VDEC_S_BASE + 8)
#define VDEC_S_ENOPREREQ (VDEC_S_BASE + 9)
#define VDEC_S_ECMDQFULL (VDEC_S_BASE + 10)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_S_ENOTSUPP (VDEC_S_BASE + 11)
#define VDEC_S_ENOTIMPL (VDEC_S_BASE + 12)
#define VDEC_S_BUSY (VDEC_S_BASE + 13)
#define VDEC_S_INPUT_BITSTREAM_ERR (VDEC_S_BASE + 14)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_INTF_VER 1
#define VDEC_MSG_BASE 0x0000000
#define VDEC_MSG_INVALID (VDEC_MSG_BASE + 0)
#define VDEC_MSG_RESP_INPUT_BUFFER_DONE (VDEC_MSG_BASE + 1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_MSG_RESP_OUTPUT_BUFFER_DONE (VDEC_MSG_BASE + 2)
#define VDEC_MSG_RESP_INPUT_FLUSHED (VDEC_MSG_BASE + 3)
#define VDEC_MSG_RESP_OUTPUT_FLUSHED (VDEC_MSG_BASE + 4)
#define VDEC_MSG_RESP_FLUSH_INPUT_DONE (VDEC_MSG_BASE + 5)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_MSG_RESP_FLUSH_OUTPUT_DONE (VDEC_MSG_BASE + 6)
#define VDEC_MSG_RESP_START_DONE (VDEC_MSG_BASE + 7)
#define VDEC_MSG_RESP_STOP_DONE (VDEC_MSG_BASE + 8)
#define VDEC_MSG_RESP_PAUSE_DONE (VDEC_MSG_BASE + 9)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_MSG_RESP_RESUME_DONE (VDEC_MSG_BASE + 10)
#define VDEC_MSG_RESP_RESOURCE_LOADED (VDEC_MSG_BASE + 11)
#define VDEC_EVT_RESOURCES_LOST (VDEC_MSG_BASE + 12)
#define VDEC_MSG_EVT_CONFIG_CHANGED (VDEC_MSG_BASE + 13)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_MSG_EVT_HW_ERROR (VDEC_MSG_BASE + 14)
#define VDEC_MSG_EVT_INFO_CONFIG_CHANGED (VDEC_MSG_BASE + 15)
#define VDEC_MSG_EVT_INFO_FIELD_DROPPED (VDEC_MSG_BASE + 16)
#define VDEC_BUFFERFLAG_EOS 0x00000001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_BUFFERFLAG_DECODEONLY 0x00000004
#define VDEC_BUFFERFLAG_DATACORRUPT 0x00000008
#define VDEC_BUFFERFLAG_ENDOFFRAME 0x00000010
#define VDEC_BUFFERFLAG_SYNCFRAME 0x00000020
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_BUFFERFLAG_EXTRADATA 0x00000040
#define VDEC_BUFFERFLAG_CODECCONFIG 0x00000080
#define VDEC_EXTRADATA_NONE 0x001
#define VDEC_EXTRADATA_QP 0x004
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_EXTRADATA_MB_ERROR_MAP 0x008
#define VDEC_EXTRADATA_SEI 0x010
#define VDEC_EXTRADATA_VUI 0x020
#define VDEC_EXTRADATA_VC1 0x040
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_CMDBASE 0x800
#define VDEC_CMD_SET_INTF_VERSION (VDEC_CMDBASE)
#define VDEC_IOCTL_MAGIC 'v'
struct vdec_ioctl_msg {
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  void __user *in;
  void __user *out;
};
#define VDEC_IOCTL_GET_PROFILE_LEVEL_SUPPORTED   _IOWR(VDEC_IOCTL_MAGIC, 0, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_GET_INTERLACE_FORMAT   _IOR(VDEC_IOCTL_MAGIC, 1, struct vdec_ioctl_msg)
#define VDEC_IOCTL_GET_CURRENT_PROFILE_LEVEL   _IOWR(VDEC_IOCTL_MAGIC, 2, struct vdec_ioctl_msg)
#define VDEC_IOCTL_SET_OUTPUT_FORMAT   _IOWR(VDEC_IOCTL_MAGIC, 3, struct vdec_ioctl_msg)
#define VDEC_IOCTL_GET_OUTPUT_FORMAT   _IOWR(VDEC_IOCTL_MAGIC, 4, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_SET_CODEC   _IOW(VDEC_IOCTL_MAGIC, 5, struct vdec_ioctl_msg)
#define VDEC_IOCTL_GET_CODEC   _IOR(VDEC_IOCTL_MAGIC, 6, struct vdec_ioctl_msg)
#define VDEC_IOCTL_SET_PICRES   _IOW(VDEC_IOCTL_MAGIC, 7, struct vdec_ioctl_msg)
#define VDEC_IOCTL_GET_PICRES   _IOR(VDEC_IOCTL_MAGIC, 8, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_SET_EXTRADATA   _IOW(VDEC_IOCTL_MAGIC, 9, struct vdec_ioctl_msg)
#define VDEC_IOCTL_GET_EXTRADATA   _IOR(VDEC_IOCTL_MAGIC, 10, struct vdec_ioctl_msg)
#define VDEC_IOCTL_SET_SEQUENCE_HEADER   _IOW(VDEC_IOCTL_MAGIC, 11, struct vdec_ioctl_msg)
#define VDEC_IOCTL_SET_BUFFER_REQ   _IOW(VDEC_IOCTL_MAGIC, 12, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_GET_BUFFER_REQ   _IOR(VDEC_IOCTL_MAGIC, 13, struct vdec_ioctl_msg)
#define VDEC_IOCTL_ALLOCATE_BUFFER   _IOWR(VDEC_IOCTL_MAGIC, 14, struct vdec_ioctl_msg)
#define VDEC_IOCTL_FREE_BUFFER   _IOW(VDEC_IOCTL_MAGIC, 15, struct vdec_ioctl_msg)
#define VDEC_IOCTL_SET_BUFFER   _IOW(VDEC_IOCTL_MAGIC, 16, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_FILL_OUTPUT_BUFFER   _IOW(VDEC_IOCTL_MAGIC, 17, struct vdec_ioctl_msg)
#define VDEC_IOCTL_DECODE_FRAME   _IOW(VDEC_IOCTL_MAGIC, 18, struct vdec_ioctl_msg)
#define VDEC_IOCTL_LOAD_RESOURCES _IO(VDEC_IOCTL_MAGIC, 19)
#define VDEC_IOCTL_CMD_START _IO(VDEC_IOCTL_MAGIC, 20)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_CMD_STOP _IO(VDEC_IOCTL_MAGIC, 21)
#define VDEC_IOCTL_CMD_PAUSE _IO(VDEC_IOCTL_MAGIC, 22)
#define VDEC_IOCTL_CMD_RESUME _IO(VDEC_IOCTL_MAGIC, 23)
#define VDEC_IOCTL_CMD_FLUSH _IOW(VDEC_IOCTL_MAGIC, 24, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_GET_NEXT_MSG   _IOR(VDEC_IOCTL_MAGIC, 25, struct vdec_ioctl_msg)
#define VDEC_IOCTL_STOP_NEXT_MSG _IO(VDEC_IOCTL_MAGIC, 26)
#define VDEC_IOCTL_GET_NUMBER_INSTANCES   _IOR(VDEC_IOCTL_MAGIC, 27, struct vdec_ioctl_msg)
#define VDEC_IOCTL_SET_PICTURE_ORDER   _IOW(VDEC_IOCTL_MAGIC, 28, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_SET_FRAME_RATE   _IOW(VDEC_IOCTL_MAGIC, 29, struct vdec_ioctl_msg)
#define VDEC_IOCTL_SET_H264_MV_BUFFER   _IOW(VDEC_IOCTL_MAGIC, 30, struct vdec_ioctl_msg)
#define VDEC_IOCTL_FREE_H264_MV_BUFFER   _IOW(VDEC_IOCTL_MAGIC, 31, struct vdec_ioctl_msg)
#define VDEC_IOCTL_GET_MV_BUFFER_SIZE   _IOR(VDEC_IOCTL_MAGIC, 32, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_SET_IDR_ONLY_DECODING   _IO(VDEC_IOCTL_MAGIC, 33)
#define VDEC_IOCTL_SET_CONT_ON_RECONFIG   _IO(VDEC_IOCTL_MAGIC, 34)
#define VDEC_IOCTL_SET_DISABLE_DMX   _IOW(VDEC_IOCTL_MAGIC, 35, struct vdec_ioctl_msg)
#define VDEC_IOCTL_GET_DISABLE_DMX   _IOR(VDEC_IOCTL_MAGIC, 36, struct vdec_ioctl_msg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VDEC_IOCTL_GET_DISABLE_DMX_SUPPORT   _IOR(VDEC_IOCTL_MAGIC, 37, struct vdec_ioctl_msg)
enum vdec_picture {
  PICTURE_TYPE_I,
  PICTURE_TYPE_P,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  PICTURE_TYPE_B,
  PICTURE_TYPE_BI,
  PICTURE_TYPE_SKIP,
  PICTURE_TYPE_IDR,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 PICTURE_TYPE_UNKNOWN
};
enum vdec_buffer {
  VDEC_BUFFER_TYPE_INPUT,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 VDEC_BUFFER_TYPE_OUTPUT
};
struct vdec_allocatorproperty {
  enum vdec_buffer buffer_type;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t mincount;
  uint32_t maxcount;
  uint32_t actualcount;
  size_t buffer_size;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t alignment;
  uint32_t buf_poolid;
};
struct vdec_bufferpayload {
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  void __user *bufferaddr;
  size_t buffer_len;
  int pmem_fd;
  size_t offset;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  size_t mmaped_size;
};
struct vdec_setbuffer_cmd {
  enum vdec_buffer buffer_type;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct vdec_bufferpayload buffer;
};
struct vdec_fillbuffer_cmd {
  struct vdec_bufferpayload buffer;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  void *client_data;
};
enum vdec_bufferflush {
  VDEC_FLUSH_TYPE_INPUT,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_FLUSH_TYPE_OUTPUT,
 VDEC_FLUSH_TYPE_ALL
};
enum vdec_codec {
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_CODECTYPE_H264 = 0x1,
  VDEC_CODECTYPE_H263 = 0x2,
  VDEC_CODECTYPE_MPEG4 = 0x3,
  VDEC_CODECTYPE_DIVX_3 = 0x4,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_CODECTYPE_DIVX_4 = 0x5,
  VDEC_CODECTYPE_DIVX_5 = 0x6,
  VDEC_CODECTYPE_DIVX_6 = 0x7,
  VDEC_CODECTYPE_XVID = 0x8,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_CODECTYPE_MPEG1 = 0x9,
  VDEC_CODECTYPE_MPEG2 = 0xa,
  VDEC_CODECTYPE_VC1 = 0xb,
 VDEC_CODECTYPE_VC1_RCV = 0xc
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum vdec_mpeg2_profile {
  VDEC_MPEG2ProfileSimple = 0x1,
  VDEC_MPEG2ProfileMain = 0x2,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG2Profile422 = 0x4,
  VDEC_MPEG2ProfileSNR = 0x8,
  VDEC_MPEG2ProfileSpatial = 0x10,
  VDEC_MPEG2ProfileHigh = 0x20,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG2ProfileKhronosExtensions = 0x6F000000,
  VDEC_MPEG2ProfileVendorStartUnused = 0x7F000000,
 VDEC_MPEG2ProfileMax = 0x7FFFFFFF
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum vdec_mpeg2_level {
  VDEC_MPEG2LevelLL = 0x1,
  VDEC_MPEG2LevelML = 0x2,
  VDEC_MPEG2LevelH14 = 0x4,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG2LevelHL = 0x8,
  VDEC_MPEG2LevelKhronosExtensions = 0x6F000000,
  VDEC_MPEG2LevelVendorStartUnused = 0x7F000000,
 VDEC_MPEG2LevelMax = 0x7FFFFFFF
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum vdec_mpeg4_profile {
  VDEC_MPEG4ProfileSimple = 0x01,
  VDEC_MPEG4ProfileSimpleScalable = 0x02,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG4ProfileCore = 0x04,
  VDEC_MPEG4ProfileMain = 0x08,
  VDEC_MPEG4ProfileNbit = 0x10,
  VDEC_MPEG4ProfileScalableTexture = 0x20,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG4ProfileSimpleFace = 0x40,
  VDEC_MPEG4ProfileSimpleFBA = 0x80,
  VDEC_MPEG4ProfileBasicAnimated = 0x100,
  VDEC_MPEG4ProfileHybrid = 0x200,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG4ProfileAdvancedRealTime = 0x400,
  VDEC_MPEG4ProfileCoreScalable = 0x800,
  VDEC_MPEG4ProfileAdvancedCoding = 0x1000,
  VDEC_MPEG4ProfileAdvancedCore = 0x2000,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG4ProfileAdvancedScalable = 0x4000,
  VDEC_MPEG4ProfileAdvancedSimple = 0x8000,
  VDEC_MPEG4ProfileKhronosExtensions = 0x6F000000,
  VDEC_MPEG4ProfileVendorStartUnused = 0x7F000000,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 VDEC_MPEG4ProfileMax = 0x7FFFFFFF
};
enum vdec_mpeg4_level {
  VDEC_MPEG4Level0 = 0x01,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG4Level0b = 0x02,
  VDEC_MPEG4Level1 = 0x04,
  VDEC_MPEG4Level2 = 0x08,
  VDEC_MPEG4Level3 = 0x10,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG4Level4 = 0x20,
  VDEC_MPEG4Level4a = 0x40,
  VDEC_MPEG4Level5 = 0x80,
  VDEC_MPEG4LevelKhronosExtensions = 0x6F000000,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_MPEG4LevelVendorStartUnused = 0x7F000000,
 VDEC_MPEG4LevelMax = 0x7FFFFFFF
};
enum vdec_avc_profile {
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_AVCProfileBaseline = 0x01,
  VDEC_AVCProfileMain = 0x02,
  VDEC_AVCProfileExtended = 0x04,
  VDEC_AVCProfileHigh = 0x08,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_AVCProfileHigh10 = 0x10,
  VDEC_AVCProfileHigh422 = 0x20,
  VDEC_AVCProfileHigh444 = 0x40,
  VDEC_AVCProfileKhronosExtensions = 0x6F000000,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_AVCProfileVendorStartUnused = 0x7F000000,
 VDEC_AVCProfileMax = 0x7FFFFFFF
};
enum vdec_avc_level {
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_AVCLevel1 = 0x01,
  VDEC_AVCLevel1b = 0x02,
  VDEC_AVCLevel11 = 0x04,
  VDEC_AVCLevel12 = 0x08,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_AVCLevel13 = 0x10,
  VDEC_AVCLevel2 = 0x20,
  VDEC_AVCLevel21 = 0x40,
  VDEC_AVCLevel22 = 0x80,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_AVCLevel3 = 0x100,
  VDEC_AVCLevel31 = 0x200,
  VDEC_AVCLevel32 = 0x400,
  VDEC_AVCLevel4 = 0x800,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_AVCLevel41 = 0x1000,
  VDEC_AVCLevel42 = 0x2000,
  VDEC_AVCLevel5 = 0x4000,
  VDEC_AVCLevel51 = 0x8000,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_AVCLevelKhronosExtensions = 0x6F000000,
  VDEC_AVCLevelVendorStartUnused = 0x7F000000,
 VDEC_AVCLevelMax = 0x7FFFFFFF
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum vdec_divx_profile {
  VDEC_DIVXProfile_qMobile = 0x01,
  VDEC_DIVXProfile_Mobile = 0x02,
  VDEC_DIVXProfile_HD = 0x04,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_DIVXProfile_Handheld = 0x08,
  VDEC_DIVXProfile_Portable = 0x10,
 VDEC_DIVXProfile_HomeTheater = 0x20
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum vdec_xvid_profile {
  VDEC_XVIDProfile_Simple = 0x1,
  VDEC_XVIDProfile_Advanced_Realtime_Simple = 0x2,
 VDEC_XVIDProfile_Advanced_Simple = 0x4
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum vdec_xvid_level {
  VDEC_XVID_LEVEL_S_L0 = 0x1,
  VDEC_XVID_LEVEL_S_L1 = 0x2,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_XVID_LEVEL_S_L2 = 0x4,
  VDEC_XVID_LEVEL_S_L3 = 0x8,
  VDEC_XVID_LEVEL_ARTS_L1 = 0x10,
  VDEC_XVID_LEVEL_ARTS_L2 = 0x20,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_XVID_LEVEL_ARTS_L3 = 0x40,
  VDEC_XVID_LEVEL_ARTS_L4 = 0x80,
  VDEC_XVID_LEVEL_AS_L0 = 0x100,
  VDEC_XVID_LEVEL_AS_L1 = 0x200,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_XVID_LEVEL_AS_L2 = 0x400,
  VDEC_XVID_LEVEL_AS_L3 = 0x800,
 VDEC_XVID_LEVEL_AS_L4 = 0x1000
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum vdec_h263profile {
  VDEC_H263ProfileBaseline = 0x01,
  VDEC_H263ProfileH320Coding = 0x02,
  VDEC_H263ProfileBackwardCompatible = 0x04,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_H263ProfileISWV2 = 0x08,
  VDEC_H263ProfileISWV3 = 0x10,
  VDEC_H263ProfileHighCompression = 0x20,
  VDEC_H263ProfileInternet = 0x40,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_H263ProfileInterlace = 0x80,
  VDEC_H263ProfileHighLatency = 0x100,
  VDEC_H263ProfileKhronosExtensions = 0x6F000000,
  VDEC_H263ProfileVendorStartUnused = 0x7F000000,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 VDEC_H263ProfileMax = 0x7FFFFFFF
};
enum vdec_h263level {
  VDEC_H263Level10 = 0x01,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_H263Level20 = 0x02,
  VDEC_H263Level30 = 0x04,
  VDEC_H263Level40 = 0x08,
  VDEC_H263Level45 = 0x10,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_H263Level50 = 0x20,
  VDEC_H263Level60 = 0x40,
  VDEC_H263Level70 = 0x80,
  VDEC_H263LevelKhronosExtensions = 0x6F000000,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_H263LevelVendorStartUnused = 0x7F000000,
 VDEC_H263LevelMax = 0x7FFFFFFF
};
enum vdec_wmv_format {
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_WMVFormatUnused = 0x01,
  VDEC_WMVFormat7 = 0x02,
  VDEC_WMVFormat8 = 0x04,
  VDEC_WMVFormat9 = 0x08,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_WMFFormatKhronosExtensions = 0x6F000000,
  VDEC_WMFFormatVendorStartUnused = 0x7F000000,
 VDEC_WMVFormatMax = 0x7FFFFFFF
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum vdec_vc1_profile {
  VDEC_VC1ProfileSimple = 0x1,
  VDEC_VC1ProfileMain = 0x2,
 VDEC_VC1ProfileAdvanced = 0x4
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum vdec_vc1_level {
  VDEC_VC1_LEVEL_S_Low = 0x1,
  VDEC_VC1_LEVEL_S_Medium = 0x2,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_VC1_LEVEL_M_Low = 0x4,
  VDEC_VC1_LEVEL_M_Medium = 0x8,
  VDEC_VC1_LEVEL_M_High = 0x10,
  VDEC_VC1_LEVEL_A_L0 = 0x20,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  VDEC_VC1_LEVEL_A_L1 = 0x40,
  VDEC_VC1_LEVEL_A_L2 = 0x80,
  VDEC_VC1_LEVEL_A_L3 = 0x100,
 VDEC_VC1_LEVEL_A_L4 = 0x200
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct vdec_profile_level {
  uint32_t profiles;
  uint32_t levels;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum vdec_interlaced_format {
  VDEC_InterlaceFrameProgressive = 0x1,
  VDEC_InterlaceInterleaveFrameTopFieldFirst = 0x2,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 VDEC_InterlaceInterleaveFrameBottomFieldFirst = 0x4
};
enum vdec_output_fromat {
  VDEC_YUV_FORMAT_NV12 = 0x1,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 VDEC_YUV_FORMAT_TILE_4x2 = 0x2
};
enum vdec_output_order {
  VDEC_ORDER_DISPLAY = 0x1,
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 VDEC_ORDER_DECODE = 0x2
};
struct vdec_picsize {
  uint32_t frame_width;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t frame_height;
  uint32_t stride;
  uint32_t scan_lines;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct vdec_seqheader {
  void __user *ptr_seqheader;
  size_t seq_header_len;
  int pmem_fd;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  size_t pmem_offset;
};
struct vdec_mberror {
  void __user *ptr_errormap;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  size_t err_mapsize;
};
struct vdec_input_frameinfo {
  void __user *bufferaddr;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  size_t offset;
  size_t datalen;
  uint32_t flags;
  int64_t timestamp;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  void *client_data;
  int pmem_fd;
  size_t pmem_offset;
  void __user *desc_addr;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t desc_size;
};
struct vdec_framesize {
  uint32_t left;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t top;
  uint32_t right;
  uint32_t bottom;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct vdec_aspectratioinfo {
  uint32_t aspect_ratio;
  uint32_t par_width;
  uint32_t par_height;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct vdec_output_frameinfo {
  void __user *bufferaddr;
  size_t offset;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  size_t len;
  uint32_t flags;
  int64_t time_stamp;
  enum vdec_picture pic_type;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  void *client_data;
  void *input_frame_clientdata;
  struct vdec_framesize framesize;
  enum vdec_interlaced_format interlaced_format;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  struct vdec_aspectratioinfo aspect_ratio_info;
};
union vdec_msgdata {
  struct vdec_output_frameinfo output_frame;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  void *input_frame_clientdata;
};
struct vdec_msginfo {
  uint32_t status_code;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  uint32_t msgcode;
  union vdec_msgdata msgdata;
  size_t msgdatasize;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct vdec_framerate {
  unsigned long fps_denominator;
  unsigned long fps_numerator;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct vdec_h264_mv{
  size_t size;
  int count;
  int pmem_fd;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int offset;
};
struct vdec_mv_buff_size{
  int width;
  /* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  int height;
  int size;
  int alignment;
};



/*******************************************************/
/******************* VIDEO4LINUX ***********************/
/*******************************************************/


/*
 * Common stuff for both V4L1 and V4L2
 * Moved from videodev.h
 */
#define VIDEO_MAX_FRAME               32
#define VIDEO_MAX_PLANES               8

#ifndef __KERNEL__

/* These defines are V4L1 specific and should not be used with the V4L2 API!
   They will be removed from this header in the future. */

#define VID_TYPE_CAPTURE	1	/* Can capture */
#define VID_TYPE_TUNER		2	/* Can tune */
#define VID_TYPE_TELETEXT	4	/* Does teletext */
#define VID_TYPE_OVERLAY	8	/* Overlay onto frame buffer */
#define VID_TYPE_CHROMAKEY	16	/* Overlay by chromakey */
#define VID_TYPE_CLIPPING	32	/* Can clip */
#define VID_TYPE_FRAMERAM	64	/* Uses the frame buffer memory */
#define VID_TYPE_SCALES		128	/* Scalable */
#define VID_TYPE_MONOCHROME	256	/* Monochrome only */
#define VID_TYPE_SUBCAPTURE	512	/* Can capture subareas of the image */
#define VID_TYPE_MPEG_DECODER	1024	/* Can decode MPEG streams */
#define VID_TYPE_MPEG_ENCODER	2048	/* Can encode MPEG streams */
#define VID_TYPE_MJPEG_DECODER	4096	/* Can decode MJPEG streams */
#define VID_TYPE_MJPEG_ENCODER	8192	/* Can encode MJPEG streams */
#endif

/*
 *	M I S C E L L A N E O U S
 */

/*  Four-character-code (FOURCC) */
#define v4l2_fourcc(a, b, c, d)\
	((__u32)(a) | ((__u32)(b) << 8) | ((__u32)(c) << 16) | ((__u32)(d) << 24))

/*
 *	E N U M S
 */
enum v4l2_field {
	V4L2_FIELD_ANY           = 0, /* driver can choose from none,
					 top, bottom, interlaced
					 depending on whatever it thinks
					 is approximate ... */
	V4L2_FIELD_NONE          = 1, /* this device has no fields ... */
	V4L2_FIELD_TOP           = 2, /* top field only */
	V4L2_FIELD_BOTTOM        = 3, /* bottom field only */
	V4L2_FIELD_INTERLACED    = 4, /* both fields interlaced */
	V4L2_FIELD_SEQ_TB        = 5, /* both fields sequential into one
					 buffer, top-bottom order */
	V4L2_FIELD_SEQ_BT        = 6, /* same as above + bottom-top order */
	V4L2_FIELD_ALTERNATE     = 7, /* both fields alternating into
					 separate buffers */
	V4L2_FIELD_INTERLACED_TB = 8, /* both fields interlaced, top field
					 first and the top field is
					 transmitted first */
	V4L2_FIELD_INTERLACED_BT = 9, /* both fields interlaced, top field
					 first and the bottom field is
					 transmitted first */
};
#define V4L2_FIELD_HAS_TOP(field)	\
	((field) == V4L2_FIELD_TOP 	||\
	 (field) == V4L2_FIELD_INTERLACED ||\
	 (field) == V4L2_FIELD_INTERLACED_TB ||\
	 (field) == V4L2_FIELD_INTERLACED_BT ||\
	 (field) == V4L2_FIELD_SEQ_TB	||\
	 (field) == V4L2_FIELD_SEQ_BT)
#define V4L2_FIELD_HAS_BOTTOM(field)	\
	((field) == V4L2_FIELD_BOTTOM 	||\
	 (field) == V4L2_FIELD_INTERLACED ||\
	 (field) == V4L2_FIELD_INTERLACED_TB ||\
	 (field) == V4L2_FIELD_INTERLACED_BT ||\
	 (field) == V4L2_FIELD_SEQ_TB	||\
	 (field) == V4L2_FIELD_SEQ_BT)
#define V4L2_FIELD_HAS_BOTH(field)	\
	((field) == V4L2_FIELD_INTERLACED ||\
	 (field) == V4L2_FIELD_INTERLACED_TB ||\
	 (field) == V4L2_FIELD_INTERLACED_BT ||\
	 (field) == V4L2_FIELD_SEQ_TB ||\
	 (field) == V4L2_FIELD_SEQ_BT)

enum v4l2_buf_type {
	V4L2_BUF_TYPE_VIDEO_CAPTURE        = 1,
	V4L2_BUF_TYPE_VIDEO_OUTPUT         = 2,
	V4L2_BUF_TYPE_VIDEO_OVERLAY        = 3,
	V4L2_BUF_TYPE_VBI_CAPTURE          = 4,
	V4L2_BUF_TYPE_VBI_OUTPUT           = 5,
	V4L2_BUF_TYPE_SLICED_VBI_CAPTURE   = 6,
	V4L2_BUF_TYPE_SLICED_VBI_OUTPUT    = 7,
#if 1
	/* Experimental */
	V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY = 8,
#endif
	V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE = 9,
	V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE  = 10,
	V4L2_BUF_TYPE_PRIVATE              = 0x80,
};

#define V4L2_TYPE_IS_MULTIPLANAR(type)			\
	((type) == V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE	\
	 || (type) == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE)

#define V4L2_TYPE_IS_OUTPUT(type)				\
	((type) == V4L2_BUF_TYPE_VIDEO_OUTPUT			\
	 || (type) == V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE		\
	 || (type) == V4L2_BUF_TYPE_VIDEO_OVERLAY		\
	 || (type) == V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY	\
	 || (type) == V4L2_BUF_TYPE_VBI_OUTPUT			\
	 || (type) == V4L2_BUF_TYPE_SLICED_VBI_OUTPUT)

enum v4l2_tuner_type {
	V4L2_TUNER_RADIO	     = 1,
	V4L2_TUNER_ANALOG_TV	     = 2,
	V4L2_TUNER_DIGITAL_TV	     = 3,
};

enum v4l2_memory {
	V4L2_MEMORY_MMAP             = 1,
	V4L2_MEMORY_USERPTR          = 2,
	V4L2_MEMORY_OVERLAY          = 3,
};

/* see also http://vektor.theorem.ca/graphics/ycbcr/ */
enum v4l2_colorspace {
	/* ITU-R 601 -- broadcast NTSC/PAL */
	V4L2_COLORSPACE_SMPTE170M     = 1,

	/* 1125-Line (US) HDTV */
	V4L2_COLORSPACE_SMPTE240M     = 2,

	/* HD and modern captures. */
	V4L2_COLORSPACE_REC709        = 3,

	/* broken BT878 extents (601, luma range 16-253 instead of 16-235) */
	V4L2_COLORSPACE_BT878         = 4,

	/* These should be useful.  Assume 601 extents. */
	V4L2_COLORSPACE_470_SYSTEM_M  = 5,
	V4L2_COLORSPACE_470_SYSTEM_BG = 6,

	/* I know there will be cameras that send this.  So, this is
	 * unspecified chromaticities and full 0-255 on each of the
	 * Y'CbCr components
	 */
	V4L2_COLORSPACE_JPEG          = 7,

	/* For RGB colourspaces, this is probably a good start. */
	V4L2_COLORSPACE_SRGB          = 8,
};

enum v4l2_priority {
	V4L2_PRIORITY_UNSET       = 0,  /* not initialized */
	V4L2_PRIORITY_BACKGROUND  = 1,
	V4L2_PRIORITY_INTERACTIVE = 2,
	V4L2_PRIORITY_RECORD      = 3,
	V4L2_PRIORITY_DEFAULT     = V4L2_PRIORITY_INTERACTIVE,
};

struct v4l2_rect {
	__s32   left;
	__s32   top;
	__s32   width;
	__s32   height;
};

struct v4l2_fract {
	__u32   numerator;
	__u32   denominator;
};

/**
  * struct v4l2_capability - Describes V4L2 device caps returned by VIDIOC_QUERYCAP
  *
  * @driver:	   name of the driver module (e.g. "bttv")
  * @card:	   name of the card (e.g. "Hauppauge WinTV")
  * @bus_info:	   name of the bus (e.g. "PCI:" + pci_name(pci_dev) )
  * @version:	   KERNEL_VERSION
  * @capabilities: capabilities of the physical device as a whole
  * @device_caps:  capabilities accessed via this particular device (node)
  * @reserved:	   reserved fields for future extensions
  */
struct v4l2_capability {
	__u8	driver[16];
	__u8	card[32];
	__u8	bus_info[32];
	__u32   version;
	__u32	capabilities;
	__u32	device_caps;
	__u32	reserved[3];
};

/* Values for 'capabilities' field */
#define V4L2_CAP_VIDEO_CAPTURE		0x00000001  /* Is a video capture device */
#define V4L2_CAP_VIDEO_OUTPUT		0x00000002  /* Is a video output device */
#define V4L2_CAP_VIDEO_OVERLAY		0x00000004  /* Can do video overlay */
#define V4L2_CAP_VBI_CAPTURE		0x00000010  /* Is a raw VBI capture device */
#define V4L2_CAP_VBI_OUTPUT		0x00000020  /* Is a raw VBI output device */
#define V4L2_CAP_SLICED_VBI_CAPTURE	0x00000040  /* Is a sliced VBI capture device */
#define V4L2_CAP_SLICED_VBI_OUTPUT	0x00000080  /* Is a sliced VBI output device */
#define V4L2_CAP_RDS_CAPTURE		0x00000100  /* RDS data capture */
#define V4L2_CAP_VIDEO_OUTPUT_OVERLAY	0x00000200  /* Can do video output overlay */
#define V4L2_CAP_HW_FREQ_SEEK		0x00000400  /* Can do hardware frequency seek  */
#define V4L2_CAP_RDS_OUTPUT		0x00000800  /* Is an RDS encoder */

/* Is a video capture device that supports multiplanar formats */
#define V4L2_CAP_VIDEO_CAPTURE_MPLANE	0x00001000
/* Is a video output device that supports multiplanar formats */
#define V4L2_CAP_VIDEO_OUTPUT_MPLANE	0x00002000

#define V4L2_CAP_TUNER			0x00010000  /* has a tuner */
#define V4L2_CAP_AUDIO			0x00020000  /* has audio support */
#define V4L2_CAP_RADIO			0x00040000  /* is a radio device */
#define V4L2_CAP_MODULATOR		0x00080000  /* has a modulator */

#define V4L2_CAP_READWRITE              0x01000000  /* read/write systemcalls */
#define V4L2_CAP_ASYNCIO                0x02000000  /* async I/O */
#define V4L2_CAP_STREAMING              0x04000000  /* streaming I/O ioctls */

#define V4L2_CAP_DEVICE_CAPS            0x80000000  /* sets device capabilities field */

/*
 *	V I D E O   I M A G E   F O R M A T
 */
struct v4l2_pix_format {
	__u32         		width;
	__u32			height;
	__u32			pixelformat;
	enum v4l2_field  	field;
	__u32            	bytesperline;	/* for padding, zero if unused */
	__u32          		sizeimage;
	enum v4l2_colorspace	colorspace;
	__u32			priv;		/* private data, depends on pixelformat */
};

/*      Pixel format         FOURCC                          depth  Description  */

/* RGB formats */
#define V4L2_PIX_FMT_RGB332  v4l2_fourcc('R', 'G', 'B', '1') /*  8  RGB-3-3-2     */
#define V4L2_PIX_FMT_RGB444  v4l2_fourcc('R', '4', '4', '4') /* 16  xxxxrrrr ggggbbbb */
#define V4L2_PIX_FMT_RGB555  v4l2_fourcc('R', 'G', 'B', 'O') /* 16  RGB-5-5-5     */
#define V4L2_PIX_FMT_RGB565  v4l2_fourcc('R', 'G', 'B', 'P') /* 16  RGB-5-6-5     */
#define V4L2_PIX_FMT_RGB555X v4l2_fourcc('R', 'G', 'B', 'Q') /* 16  RGB-5-5-5 BE  */
#define V4L2_PIX_FMT_RGB565X v4l2_fourcc('R', 'G', 'B', 'R') /* 16  RGB-5-6-5 BE  */
#define V4L2_PIX_FMT_BGR666  v4l2_fourcc('B', 'G', 'R', 'H') /* 18  BGR-6-6-6	  */
#define V4L2_PIX_FMT_BGR24   v4l2_fourcc('B', 'G', 'R', '3') /* 24  BGR-8-8-8     */
#define V4L2_PIX_FMT_RGB24   v4l2_fourcc('R', 'G', 'B', '3') /* 24  RGB-8-8-8     */
#define V4L2_PIX_FMT_BGR32   v4l2_fourcc('B', 'G', 'R', '4') /* 32  BGR-8-8-8-8   */
#define V4L2_PIX_FMT_RGB32   v4l2_fourcc('R', 'G', 'B', '4') /* 32  RGB-8-8-8-8   */

/* Grey formats */
#define V4L2_PIX_FMT_GREY    v4l2_fourcc('G', 'R', 'E', 'Y') /*  8  Greyscale     */
#define V4L2_PIX_FMT_Y4      v4l2_fourcc('Y', '0', '4', ' ') /*  4  Greyscale     */
#define V4L2_PIX_FMT_Y6      v4l2_fourcc('Y', '0', '6', ' ') /*  6  Greyscale     */
#define V4L2_PIX_FMT_Y10     v4l2_fourcc('Y', '1', '0', ' ') /* 10  Greyscale     */
#define V4L2_PIX_FMT_Y12     v4l2_fourcc('Y', '1', '2', ' ') /* 12  Greyscale     */
#define V4L2_PIX_FMT_Y16     v4l2_fourcc('Y', '1', '6', ' ') /* 16  Greyscale     */

/* Grey bit-packed formats */
#define V4L2_PIX_FMT_Y10BPACK    v4l2_fourcc('Y', '1', '0', 'B') /* 10  Greyscale bit-packed */

/* Palette formats */
#define V4L2_PIX_FMT_PAL8    v4l2_fourcc('P', 'A', 'L', '8') /*  8  8-bit palette */

/* Luminance+Chrominance formats */
#define V4L2_PIX_FMT_YVU410  v4l2_fourcc('Y', 'V', 'U', '9') /*  9  YVU 4:1:0     */
#define V4L2_PIX_FMT_YVU420  v4l2_fourcc('Y', 'V', '1', '2') /* 12  YVU 4:2:0     */
#define V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y', 'U', 'Y', 'V') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YYUV    v4l2_fourcc('Y', 'Y', 'U', 'V') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YVYU    v4l2_fourcc('Y', 'V', 'Y', 'U') /* 16 YVU 4:2:2 */
#define V4L2_PIX_FMT_UYVY    v4l2_fourcc('U', 'Y', 'V', 'Y') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_VYUY    v4l2_fourcc('V', 'Y', 'U', 'Y') /* 16  YUV 4:2:2     */
#define V4L2_PIX_FMT_YUV422P v4l2_fourcc('4', '2', '2', 'P') /* 16  YVU422 planar */
#define V4L2_PIX_FMT_YUV411P v4l2_fourcc('4', '1', '1', 'P') /* 16  YVU411 planar */
#define V4L2_PIX_FMT_Y41P    v4l2_fourcc('Y', '4', '1', 'P') /* 12  YUV 4:1:1     */
#define V4L2_PIX_FMT_YUV444  v4l2_fourcc('Y', '4', '4', '4') /* 16  xxxxyyyy uuuuvvvv */
#define V4L2_PIX_FMT_YUV555  v4l2_fourcc('Y', 'U', 'V', 'O') /* 16  YUV-5-5-5     */
#define V4L2_PIX_FMT_YUV565  v4l2_fourcc('Y', 'U', 'V', 'P') /* 16  YUV-5-6-5     */
#define V4L2_PIX_FMT_YUV32   v4l2_fourcc('Y', 'U', 'V', '4') /* 32  YUV-8-8-8-8   */
#define V4L2_PIX_FMT_YUV410  v4l2_fourcc('Y', 'U', 'V', '9') /*  9  YUV 4:1:0     */
#define V4L2_PIX_FMT_YUV420  v4l2_fourcc('Y', 'U', '1', '2') /* 12  YUV 4:2:0     */
#define V4L2_PIX_FMT_HI240   v4l2_fourcc('H', 'I', '2', '4') /*  8  8-bit color   */
#define V4L2_PIX_FMT_HM12    v4l2_fourcc('H', 'M', '1', '2') /*  8  YUV 4:2:0 16x16 macroblocks */
#define V4L2_PIX_FMT_M420    v4l2_fourcc('M', '4', '2', '0') /* 12  YUV 4:2:0 2 lines y, 1 line uv interleaved */

/* two planes -- one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12    v4l2_fourcc('N', 'V', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV21    v4l2_fourcc('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */
#define V4L2_PIX_FMT_NV16    v4l2_fourcc('N', 'V', '1', '6') /* 16  Y/CbCr 4:2:2  */
#define V4L2_PIX_FMT_NV61    v4l2_fourcc('N', 'V', '6', '1') /* 16  Y/CrCb 4:2:2  */
#define V4L2_PIX_FMT_NV24    v4l2_fourcc('N', 'V', '2', '4') /* 24  Y/CbCr 4:4:4  */
#define V4L2_PIX_FMT_NV42    v4l2_fourcc('N', 'V', '4', '2') /* 24  Y/CrCb 4:4:4  */

/* two non contiguous planes - one Y, one Cr + Cb interleaved  */
#define V4L2_PIX_FMT_NV12M   v4l2_fourcc('N', 'M', '1', '2') /* 12  Y/CbCr 4:2:0  */
#define V4L2_PIX_FMT_NV12MT  v4l2_fourcc('T', 'M', '1', '2') /* 12  Y/CbCr 4:2:0 64x32 macroblocks */

/* three non contiguous planes - Y, Cb, Cr */
#define V4L2_PIX_FMT_YUV420M v4l2_fourcc('Y', 'M', '1', '2') /* 12  YUV420 planar */

/* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
#define V4L2_PIX_FMT_SBGGR8  v4l2_fourcc('B', 'A', '8', '1') /*  8  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG8  v4l2_fourcc('G', 'B', 'R', 'G') /*  8  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG8  v4l2_fourcc('G', 'R', 'B', 'G') /*  8  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB8  v4l2_fourcc('R', 'G', 'G', 'B') /*  8  RGRG.. GBGB.. */
#define V4L2_PIX_FMT_SBGGR10 v4l2_fourcc('B', 'G', '1', '0') /* 10  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG10 v4l2_fourcc('G', 'B', '1', '0') /* 10  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG10 v4l2_fourcc('B', 'A', '1', '0') /* 10  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB10 v4l2_fourcc('R', 'G', '1', '0') /* 10  RGRG.. GBGB.. */
#define V4L2_PIX_FMT_SBGGR12 v4l2_fourcc('B', 'G', '1', '2') /* 12  BGBG.. GRGR.. */
#define V4L2_PIX_FMT_SGBRG12 v4l2_fourcc('G', 'B', '1', '2') /* 12  GBGB.. RGRG.. */
#define V4L2_PIX_FMT_SGRBG12 v4l2_fourcc('B', 'A', '1', '2') /* 12  GRGR.. BGBG.. */
#define V4L2_PIX_FMT_SRGGB12 v4l2_fourcc('R', 'G', '1', '2') /* 12  RGRG.. GBGB.. */
	/* 10bit raw bayer DPCM compressed to 8 bits */
#define V4L2_PIX_FMT_SGRBG10DPCM8 v4l2_fourcc('B', 'D', '1', '0')
	/*
	 * 10bit raw bayer, expanded to 16 bits
	 * xxxxrrrrrrrrrrxxxxgggggggggg xxxxggggggggggxxxxbbbbbbbbbb...
	 */
#define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B', 'Y', 'R', '2') /* 16  BGBG.. GRGR.. */

/* compressed formats */
#define V4L2_PIX_FMT_MJPEG    v4l2_fourcc('M', 'J', 'P', 'G') /* Motion-JPEG   */
#define V4L2_PIX_FMT_JPEG     v4l2_fourcc('J', 'P', 'E', 'G') /* JFIF JPEG     */
#define V4L2_PIX_FMT_DV       v4l2_fourcc('d', 'v', 's', 'd') /* 1394          */
#define V4L2_PIX_FMT_MPEG     v4l2_fourcc('M', 'P', 'E', 'G') /* MPEG-1/2/4 Multiplexed */
#define V4L2_PIX_FMT_H264     v4l2_fourcc('H', '2', '6', '4') /* H264 with start codes */
#define V4L2_PIX_FMT_H264_NO_SC v4l2_fourcc('A', 'V', 'C', '1') /* H264 without start codes */
#define V4L2_PIX_FMT_H263     v4l2_fourcc('H', '2', '6', '3') /* H263          */
#define V4L2_PIX_FMT_MPEG1    v4l2_fourcc('M', 'P', 'G', '1') /* MPEG-1 ES     */
#define V4L2_PIX_FMT_MPEG2    v4l2_fourcc('M', 'P', 'G', '2') /* MPEG-2 ES     */
#define V4L2_PIX_FMT_MPEG4    v4l2_fourcc('M', 'P', 'G', '4') /* MPEG-4 ES     */
#define V4L2_PIX_FMT_XVID     v4l2_fourcc('X', 'V', 'I', 'D') /* Xvid           */
#define V4L2_PIX_FMT_VC1_ANNEX_G v4l2_fourcc('V', 'C', '1', 'G') /* SMPTE 421M Annex G compliant stream */
#define V4L2_PIX_FMT_VC1_ANNEX_L v4l2_fourcc('V', 'C', '1', 'L') /* SMPTE 421M Annex L compliant stream */
#define V4L2_PIX_FMT_DIVX_311  v4l2_fourcc('D', 'I', 'V', '3') /* DIVX311     */
#define V4L2_PIX_FMT_DIVX      v4l2_fourcc('D', 'I', 'V', 'X') /* DIVX        */
#define V4L2_PIX_FMT_VP8 v4l2_fourcc('V', 'P', '8', '0') /* ON2 VP8 stream */
#define V4L2_PIX_FMT_HEVC v4l2_fourcc('H', 'E', 'V', 'C') /* for HEVC stream */
#define V4L2_PIX_FMT_HEVC_HYBRID v4l2_fourcc('H', 'V', 'C', 'H')

/*  Vendor-specific formats   */
#define V4L2_PIX_FMT_CPIA1    v4l2_fourcc('C', 'P', 'I', 'A') /* cpia1 YUV */
#define V4L2_PIX_FMT_WNVA     v4l2_fourcc('W', 'N', 'V', 'A') /* Winnov hw compress */
#define V4L2_PIX_FMT_SN9C10X  v4l2_fourcc('S', '9', '1', '0') /* SN9C10x compression */
#define V4L2_PIX_FMT_SN9C20X_I420 v4l2_fourcc('S', '9', '2', '0') /* SN9C20x YUV 4:2:0 */
#define V4L2_PIX_FMT_PWC1     v4l2_fourcc('P', 'W', 'C', '1') /* pwc older webcam */
#define V4L2_PIX_FMT_PWC2     v4l2_fourcc('P', 'W', 'C', '2') /* pwc newer webcam */
#define V4L2_PIX_FMT_ET61X251 v4l2_fourcc('E', '6', '2', '5') /* ET61X251 compression */
#define V4L2_PIX_FMT_SPCA501  v4l2_fourcc('S', '5', '0', '1') /* YUYV per line */
#define V4L2_PIX_FMT_SPCA505  v4l2_fourcc('S', '5', '0', '5') /* YYUV per line */
#define V4L2_PIX_FMT_SPCA508  v4l2_fourcc('S', '5', '0', '8') /* YUVY per line */
#define V4L2_PIX_FMT_SPCA561  v4l2_fourcc('S', '5', '6', '1') /* compressed GBRG bayer */
#define V4L2_PIX_FMT_PAC207   v4l2_fourcc('P', '2', '0', '7') /* compressed BGGR bayer */
#define V4L2_PIX_FMT_MR97310A v4l2_fourcc('M', '3', '1', '0') /* compressed BGGR bayer */
#define V4L2_PIX_FMT_JL2005BCD v4l2_fourcc('J', 'L', '2', '0') /* compressed RGGB bayer */
#define V4L2_PIX_FMT_SN9C2028 v4l2_fourcc('S', 'O', 'N', 'X') /* compressed GBRG bayer */
#define V4L2_PIX_FMT_SQ905C   v4l2_fourcc('9', '0', '5', 'C') /* compressed RGGB bayer */
#define V4L2_PIX_FMT_PJPG     v4l2_fourcc('P', 'J', 'P', 'G') /* Pixart 73xx JPEG */
#define V4L2_PIX_FMT_OV511    v4l2_fourcc('O', '5', '1', '1') /* ov511 JPEG */
#define V4L2_PIX_FMT_OV518    v4l2_fourcc('O', '5', '1', '8') /* ov518 JPEG */
#define V4L2_PIX_FMT_STV0680  v4l2_fourcc('S', '6', '8', '0') /* stv0680 bayer */
#define V4L2_PIX_FMT_TM6000   v4l2_fourcc('T', 'M', '6', '0') /* tm5600/tm60x0 */
#define V4L2_PIX_FMT_CIT_YYVYUY v4l2_fourcc('C', 'I', 'T', 'V') /* one line of Y then 1 line of VYUY */
#define V4L2_PIX_FMT_KONICA420  v4l2_fourcc('K', 'O', 'N', 'I') /* YUV420 planar in blocks of 256 pixels */
#define V4L2_PIX_FMT_JPGL	v4l2_fourcc('J', 'P', 'G', 'L') /* JPEG-Lite */
/* se401 janggu compressed rgb */
#define V4L2_PIX_FMT_SE401      v4l2_fourcc('S', '4', '0', '1')
/* Composite stats */
#define V4L2_PIX_FMT_STATS_COMB v4l2_fourcc('S', 'T', 'C', 'M')
/* AEC stats */
#define V4L2_PIX_FMT_STATS_AE   v4l2_fourcc('S', 'T', 'A', 'E')
/* AF stats */
#define V4L2_PIX_FMT_STATS_AF   v4l2_fourcc('S', 'T', 'A', 'F')
/* AWB stats */
#define V4L2_PIX_FMT_STATS_AWB  v4l2_fourcc('S', 'T', 'W', 'B')
/* IHIST stats */
#define V4L2_PIX_FMT_STATS_IHST v4l2_fourcc('I', 'H', 'S', 'T')
/* Column count stats */
#define V4L2_PIX_FMT_STATS_CS   v4l2_fourcc('S', 'T', 'C', 'S')
/* Row count stats */
#define V4L2_PIX_FMT_STATS_RS   v4l2_fourcc('S', 'T', 'R', 'S')
/* Bayer Grid stats */
#define V4L2_PIX_FMT_STATS_BG   v4l2_fourcc('S', 'T', 'B', 'G')
/* Bayer focus stats */
#define V4L2_PIX_FMT_STATS_BF   v4l2_fourcc('S', 'T', 'B', 'F')
/* Bayer hist stats */
#define V4L2_PIX_FMT_STATS_BHST v4l2_fourcc('B', 'H', 'S', 'T')

/*
 *	F O R M A T   E N U M E R A T I O N
 */
struct v4l2_fmtdesc {
	__u32		    index;             /* Format number      */
	enum v4l2_buf_type  type;              /* buffer type        */
	__u32               flags;
	__u8		    description[32];   /* Description string */
	__u32		    pixelformat;       /* Format fourcc      */
	__u32		    reserved[4];
};

#define V4L2_FMT_FLAG_COMPRESSED 0x0001
#define V4L2_FMT_FLAG_EMULATED   0x0002

#if 1
	/* Experimental Frame Size and frame rate enumeration */
/*
 *	F R A M E   S I Z E   E N U M E R A T I O N
 */
enum v4l2_frmsizetypes {
	V4L2_FRMSIZE_TYPE_DISCRETE	= 1,
	V4L2_FRMSIZE_TYPE_CONTINUOUS	= 2,
	V4L2_FRMSIZE_TYPE_STEPWISE	= 3,
};

struct v4l2_frmsize_discrete {
	__u32			width;		/* Frame width [pixel] */
	__u32			height;		/* Frame height [pixel] */
};

struct v4l2_frmsize_stepwise {
	__u32			min_width;	/* Minimum frame width [pixel] */
	__u32			max_width;	/* Maximum frame width [pixel] */
	__u32			step_width;	/* Frame width step size [pixel] */
	__u32			min_height;	/* Minimum frame height [pixel] */
	__u32			max_height;	/* Maximum frame height [pixel] */
	__u32			step_height;	/* Frame height step size [pixel] */
};

struct v4l2_frmsizeenum {
	__u32			index;		/* Frame size number */
	__u32			pixel_format;	/* Pixel format */
	__u32			type;		/* Frame size type the device supports. */

	union {					/* Frame size */
		struct v4l2_frmsize_discrete	discrete;
		struct v4l2_frmsize_stepwise	stepwise;
	};

	__u32   reserved[2];			/* Reserved space for future use */
};

/*
 *	F R A M E   R A T E   E N U M E R A T I O N
 */
enum v4l2_frmivaltypes {
	V4L2_FRMIVAL_TYPE_DISCRETE	= 1,
	V4L2_FRMIVAL_TYPE_CONTINUOUS	= 2,
	V4L2_FRMIVAL_TYPE_STEPWISE	= 3,
};

struct v4l2_frmival_stepwise {
	struct v4l2_fract	min;		/* Minimum frame interval [s] */
	struct v4l2_fract	max;		/* Maximum frame interval [s] */
	struct v4l2_fract	step;		/* Frame interval step size [s] */
};

struct v4l2_frmivalenum {
	__u32			index;		/* Frame format index */
	__u32			pixel_format;	/* Pixel format */
	__u32			width;		/* Frame width */
	__u32			height;		/* Frame height */
	__u32			type;		/* Frame interval type the device supports. */

	union {					/* Frame interval */
		struct v4l2_fract		discrete;
		struct v4l2_frmival_stepwise	stepwise;
	};

	__u32	reserved[2];			/* Reserved space for future use */
};
#endif

/*
 *	T I M E C O D E
 */
struct v4l2_timecode {
	__u32	type;
	__u32	flags;
	__u8	frames;
	__u8	seconds;
	__u8	minutes;
	__u8	hours;
	__u8	userbits[4];
};

/*  Type  */
#define V4L2_TC_TYPE_24FPS		1
#define V4L2_TC_TYPE_25FPS		2
#define V4L2_TC_TYPE_30FPS		3
#define V4L2_TC_TYPE_50FPS		4
#define V4L2_TC_TYPE_60FPS		5

/*  Flags  */
#define V4L2_TC_FLAG_DROPFRAME		0x0001 /* "drop-frame" mode */
#define V4L2_TC_FLAG_COLORFRAME		0x0002
#define V4L2_TC_USERBITS_field		0x000C
#define V4L2_TC_USERBITS_USERDEFINED	0x0000
#define V4L2_TC_USERBITS_8BITCHARS	0x0008
/* The above is based on SMPTE timecodes */

struct v4l2_jpegcompression {
	int quality;

	int  APPn;              /* Number of APP segment to be written,
				 * must be 0..15 */
	int  APP_len;           /* Length of data in JPEG APPn segment */
	char APP_data[60];      /* Data in the JPEG APPn segment. */

	int  COM_len;           /* Length of data in JPEG COM segment */
	char COM_data[60];      /* Data in JPEG COM segment */

	__u32 jpeg_markers;     /* Which markers should go into the JPEG
				 * output. Unless you exactly know what
				 * you do, leave them untouched.
				 * Inluding less markers will make the
				 * resulting code smaller, but there will
				 * be fewer applications which can read it.
				 * The presence of the APP and COM marker
				 * is influenced by APP_len and COM_len
				 * ONLY, not by this property! */

#define V4L2_JPEG_MARKER_DHT (1<<3)    /* Define Huffman Tables */
#define V4L2_JPEG_MARKER_DQT (1<<4)    /* Define Quantization Tables */
#define V4L2_JPEG_MARKER_DRI (1<<5)    /* Define Restart Interval */
#define V4L2_JPEG_MARKER_COM (1<<6)    /* Comment segment */
#define V4L2_JPEG_MARKER_APP (1<<7)    /* App segment, driver will
					* allways use APP0 */
};

/*
 *	M E M O R Y - M A P P I N G   B U F F E R S
 */
struct v4l2_requestbuffers {
	__u32			count;
	enum v4l2_buf_type      type;
	enum v4l2_memory        memory;
	__u32			reserved[2];
};

/**
 * struct v4l2_plane - plane info for multi-planar buffers
 * @bytesused:		number of bytes occupied by data in the plane (payload)
 * @length:		size of this plane (NOT the payload) in bytes
 * @mem_offset:		when memory in the associated struct v4l2_buffer is
 *			V4L2_MEMORY_MMAP, equals the offset from the start of
 *			the device memory for this plane (or is a "cookie" that
 *			should be passed to mmap() called on the video node)
 * @userptr:		when memory is V4L2_MEMORY_USERPTR, a userspace pointer
 *			pointing to this plane
 * @data_offset:	offset in the plane to the start of data; usually 0,
 *			unless there is a header in front of the data
 *
 * Multi-planar buffers consist of one or more planes, e.g. an YCbCr buffer
 * with two planes can have one plane for Y, and another for interleaved CbCr
 * components. Each plane can reside in a separate memory buffer, or even in
 * a completely separate memory node (e.g. in embedded devices).
 */
struct v4l2_plane {
	__u32			bytesused;
	__u32			length;
	  unsigned long	userptr;
	__u32			data_offset;
	__u32			reserved[11];
};

/**
 * struct v4l2_buffer - video buffer info
 * @index:	id number of the buffer
 * @type:	buffer type (type == *_MPLANE for multiplanar buffers)
 * @bytesused:	number of bytes occupied by data in the buffer (payload);
 *		unused (set to 0) for multiplanar buffers
 * @flags:	buffer informational flags
 * @field:	field order of the image in the buffer
 * @timestamp:	frame timestamp
 * @timecode:	frame timecode
 * @sequence:	sequence count of this frame
 * @memory:	the method, in which the actual video data is passed
 * @offset:	for non-multiplanar buffers with memory == V4L2_MEMORY_MMAP;
 *		offset from the start of the device memory for this plane,
 *		(or a "cookie" that should be passed to mmap() as offset)
 * @userptr:	for non-multiplanar buffers with memory == V4L2_MEMORY_USERPTR;
 *		a userspace pointer pointing to this buffer
 * @planes:	for multiplanar buffers; userspace pointer to the array of plane
 *		info structs for this buffer
 * @length:	size in bytes of the buffer (NOT its payload) for single-plane
 *		buffers (when type != *_MPLANE); number of elements in the
 *		planes array for multi-plane buffers
 * @input:	input number from which the video data has has been captured
 *
 * Contains data exchanged by application and driver using one of the Streaming
 * I/O methods.
 */
struct v4l2_buffer {
	__u32			index;
	enum v4l2_buf_type      type;
	__u32			bytesused;
	__u32			flags;
	enum v4l2_field		field;
	struct timeval		timestamp;
	struct v4l2_timecode	timecode;
	__u32			sequence;

	/* memory location */
	enum v4l2_memory        memory;
  struct v4l2_plane *planes;

	__u32			length;
	__u32			input;
	__u32			reserved;
};

/*  Flags for 'flags' field */
#define V4L2_BUF_FLAG_MAPPED	0x0001  /* Buffer is mapped (flag) */
#define V4L2_BUF_FLAG_QUEUED	0x0002	/* Buffer is queued for processing */
#define V4L2_BUF_FLAG_DONE	0x0004	/* Buffer is ready */
#define V4L2_BUF_FLAG_KEYFRAME	0x0008	/* Image is a keyframe (I-frame) */
#define V4L2_BUF_FLAG_PFRAME	0x0010	/* Image is a P-frame */
#define V4L2_BUF_FLAG_BFRAME	0x0020	/* Image is a B-frame */
/* Buffer is ready, but the data contained within is corrupted. */
#define V4L2_BUF_FLAG_ERROR	0x0040
#define V4L2_BUF_FLAG_TIMECODE	0x0100	/* timecode field is valid */
#define V4L2_BUF_FLAG_INPUT     0x0200  /* input field is valid */
#define V4L2_BUF_FLAG_PREPARED	0x0400	/* Buffer is prepared for queuing */
/* Cache handling flags */
#define V4L2_BUF_FLAG_NO_CACHE_INVALIDATE	0x0800
#define V4L2_BUF_FLAG_NO_CACHE_CLEAN		0x1000
#define V4L2_BUF_FLAG_EOS		0x2000
#define V4L2_QCOM_BUF_FLAG_CODECCONFIG  0x4000
#define V4L2_QCOM_BUF_FLAG_EOSEQ  0x8000
#define V4L2_QCOM_BUF_TIMESTAMP_INVALID 0x10000
#define V4L2_QCOM_BUF_FLAG_IDRFRAME	0x20000	/* Image is a IDR-frame */
#define V4L2_QCOM_BUF_FLAG_DECODEONLY 0x40000
#define V4L2_QCOM_BUF_DATA_CORRUPT 0x80000
#define V4L2_QCOM_BUF_DROP_FRAME 0x100000
#define V4L2_QCOM_BUF_INPUT_UNSUPPORTED 0x200000
#define V4L2_QCOM_BUF_FLAG_EOS          0x2000
#define V4L2_QCOM_BUF_FLAG_READONLY     0x400000

/*
 *	O V E R L A Y   P R E V I E W
 */
struct v4l2_framebuffer {
	__u32			capability;
	__u32			flags;
/* FIXME: in theory we should pass something like PCI device + memory
 * region + offset instead of some physical address */
	void                    *base;
	struct v4l2_pix_format	fmt;
};
/*  Flags for the 'capability' field. Read only */
#define V4L2_FBUF_CAP_EXTERNOVERLAY	0x0001
#define V4L2_FBUF_CAP_CHROMAKEY		0x0002
#define V4L2_FBUF_CAP_LIST_CLIPPING     0x0004
#define V4L2_FBUF_CAP_BITMAP_CLIPPING	0x0008
#define V4L2_FBUF_CAP_LOCAL_ALPHA	0x0010
#define V4L2_FBUF_CAP_GLOBAL_ALPHA	0x0020
#define V4L2_FBUF_CAP_LOCAL_INV_ALPHA	0x0040
#define V4L2_FBUF_CAP_SRC_CHROMAKEY	0x0080
/*  Flags for the 'flags' field. */
#define V4L2_FBUF_FLAG_PRIMARY		0x0001
#define V4L2_FBUF_FLAG_OVERLAY		0x0002
#define V4L2_FBUF_FLAG_CHROMAKEY	0x0004
#define V4L2_FBUF_FLAG_LOCAL_ALPHA	0x0008
#define V4L2_FBUF_FLAG_GLOBAL_ALPHA	0x0010
#define V4L2_FBUF_FLAG_LOCAL_INV_ALPHA	0x0020
#define V4L2_FBUF_FLAG_SRC_CHROMAKEY	0x0040

struct v4l2_clip {
	struct v4l2_rect        c;
	struct v4l2_clip	__user *next;
};

struct v4l2_window {
	struct v4l2_rect        w;
	enum v4l2_field  	field;
	__u32			chromakey;
	struct v4l2_clip	__user *clips;
	__u32			clipcount;
	void			__user *bitmap;
	__u8                    global_alpha;
};

/*
 *	C A P T U R E   P A R A M E T E R S
 */
struct v4l2_captureparm {
	__u32		   capability;	  /*  Supported modes */
	__u32		   capturemode;	  /*  Current mode */
	struct v4l2_fract  timeperframe;  /*  Time per frame in .1us units */
	__u32		   extendedmode;  /*  Driver-specific extensions */
	__u32              readbuffers;   /*  # of buffers for read */
	__u32		   reserved[4];
};

/*  Flags for 'capability' and 'capturemode' fields */
#define V4L2_MODE_HIGHQUALITY	0x0001	/*  High quality imaging mode */
#define V4L2_CAP_TIMEPERFRAME	0x1000	/*  timeperframe field is supported */
#define V4L2_CAP_QCOM_FRAMESKIP	0x2000	/*  frame skipping is supported */

struct v4l2_qcom_frameskip {
	__u64		   maxframeinterval;
	__u8		   fpsvariance;
};

struct v4l2_outputparm {
	__u32		   capability;	 /*  Supported modes */
	__u32		   outputmode;	 /*  Current mode */
	struct v4l2_fract  timeperframe; /*  Time per frame in seconds */
	__u32		   extendedmode; /*  Driver-specific extensions */
	__u32              writebuffers; /*  # of buffers for write */
	__u32		   reserved[4];
};

/*
 *	I N P U T   I M A G E   C R O P P I N G
 */
struct v4l2_cropcap {
	enum v4l2_buf_type      type;
	struct v4l2_rect        bounds;
	struct v4l2_rect        defrect;
	struct v4l2_fract       pixelaspect;
};

struct v4l2_crop {
	enum v4l2_buf_type      type;
	struct v4l2_rect        c;
};

/* Hints for adjustments of selection rectangle */
#define V4L2_SEL_FLAG_GE	0x00000001
#define V4L2_SEL_FLAG_LE	0x00000002

/* Selection targets */

/* Current cropping area */
#define V4L2_SEL_TGT_CROP_ACTIVE	0x0000
/* Default cropping area */
#define V4L2_SEL_TGT_CROP_DEFAULT	0x0001
/* Cropping bounds */
#define V4L2_SEL_TGT_CROP_BOUNDS	0x0002
/* Current composing area */
#define V4L2_SEL_TGT_COMPOSE_ACTIVE	0x0100
/* Default composing area */
#define V4L2_SEL_TGT_COMPOSE_DEFAULT	0x0101
/* Composing bounds */
#define V4L2_SEL_TGT_COMPOSE_BOUNDS	0x0102
/* Current composing area plus all padding pixels */
#define V4L2_SEL_TGT_COMPOSE_PADDED	0x0103

/**
 * struct v4l2_selection - selection info
 * @type:	buffer type (do not use *_MPLANE types)
 * @target:	selection target, used to choose one of possible rectangles
 * @flags:	constraints flags
 * @r:		coordinates of selection window
 * @reserved:	for future use, rounds structure size to 64 bytes, set to zero
 *
 * Hardware may use multiple helper windows to process a video stream.
 * The structure is used to exchange this selection areas between
 * an application and a driver.
 */
struct v4l2_selection {
	__u32			type;
	__u32			target;
	__u32                   flags;
	struct v4l2_rect        r;
	__u32                   reserved[9];
};


/*
 *      A N A L O G   V I D E O   S T A N D A R D
 */

typedef __u64 v4l2_std_id;

/* one bit for each */
#define V4L2_STD_PAL_B          ((v4l2_std_id)0x00000001)
#define V4L2_STD_PAL_B1         ((v4l2_std_id)0x00000002)
#define V4L2_STD_PAL_G          ((v4l2_std_id)0x00000004)
#define V4L2_STD_PAL_H          ((v4l2_std_id)0x00000008)
#define V4L2_STD_PAL_I          ((v4l2_std_id)0x00000010)
#define V4L2_STD_PAL_D          ((v4l2_std_id)0x00000020)
#define V4L2_STD_PAL_D1         ((v4l2_std_id)0x00000040)
#define V4L2_STD_PAL_K          ((v4l2_std_id)0x00000080)

#define V4L2_STD_PAL_M          ((v4l2_std_id)0x00000100)
#define V4L2_STD_PAL_N          ((v4l2_std_id)0x00000200)
#define V4L2_STD_PAL_Nc         ((v4l2_std_id)0x00000400)
#define V4L2_STD_PAL_60         ((v4l2_std_id)0x00000800)

#define V4L2_STD_NTSC_M         ((v4l2_std_id)0x00001000)	/* BTSC */
#define V4L2_STD_NTSC_M_JP      ((v4l2_std_id)0x00002000)	/* EIA-J */
#define V4L2_STD_NTSC_443       ((v4l2_std_id)0x00004000)
#define V4L2_STD_NTSC_M_KR      ((v4l2_std_id)0x00008000)	/* FM A2 */

#define V4L2_STD_SECAM_B        ((v4l2_std_id)0x00010000)
#define V4L2_STD_SECAM_D        ((v4l2_std_id)0x00020000)
#define V4L2_STD_SECAM_G        ((v4l2_std_id)0x00040000)
#define V4L2_STD_SECAM_H        ((v4l2_std_id)0x00080000)
#define V4L2_STD_SECAM_K        ((v4l2_std_id)0x00100000)
#define V4L2_STD_SECAM_K1       ((v4l2_std_id)0x00200000)
#define V4L2_STD_SECAM_L        ((v4l2_std_id)0x00400000)
#define V4L2_STD_SECAM_LC       ((v4l2_std_id)0x00800000)

/* ATSC/HDTV */
#define V4L2_STD_ATSC_8_VSB     ((v4l2_std_id)0x01000000)
#define V4L2_STD_ATSC_16_VSB    ((v4l2_std_id)0x02000000)

/* FIXME:
   Although std_id is 64 bits, there is an issue on PPC32 architecture that
   makes switch(__u64) to break. So, there's a hack on v4l2-common.c rounding
   this value to 32 bits.
   As, currently, the max value is for V4L2_STD_ATSC_16_VSB (30 bits wide),
   it should work fine. However, if needed to add more than two standards,
   v4l2-common.c should be fixed.
 */

/*
 * Some macros to merge video standards in order to make live easier for the
 * drivers and V4L2 applications
 */

/*
 * "Common" NTSC/M - It should be noticed that V4L2_STD_NTSC_443 is
 * Missing here.
 */
#define V4L2_STD_NTSC           (V4L2_STD_NTSC_M	|\
				 V4L2_STD_NTSC_M_JP     |\
				 V4L2_STD_NTSC_M_KR)
/* Secam macros */
#define V4L2_STD_SECAM_DK      	(V4L2_STD_SECAM_D	|\
				 V4L2_STD_SECAM_K	|\
				 V4L2_STD_SECAM_K1)
/* All Secam Standards */
#define V4L2_STD_SECAM		(V4L2_STD_SECAM_B	|\
				 V4L2_STD_SECAM_G	|\
				 V4L2_STD_SECAM_H	|\
				 V4L2_STD_SECAM_DK	|\
				 V4L2_STD_SECAM_L       |\
				 V4L2_STD_SECAM_LC)
/* PAL macros */
#define V4L2_STD_PAL_BG		(V4L2_STD_PAL_B		|\
				 V4L2_STD_PAL_B1	|\
				 V4L2_STD_PAL_G)
#define V4L2_STD_PAL_DK		(V4L2_STD_PAL_D		|\
				 V4L2_STD_PAL_D1	|\
				 V4L2_STD_PAL_K)
/*
 * "Common" PAL - This macro is there to be compatible with the old
 * V4L1 concept of "PAL": /BGDKHI.
 * Several PAL standards are mising here: /M, /N and /Nc
 */
#define V4L2_STD_PAL		(V4L2_STD_PAL_BG	|\
				 V4L2_STD_PAL_DK	|\
				 V4L2_STD_PAL_H		|\
				 V4L2_STD_PAL_I)
/* Chroma "agnostic" standards */
#define V4L2_STD_B		(V4L2_STD_PAL_B		|\
				 V4L2_STD_PAL_B1	|\
				 V4L2_STD_SECAM_B)
#define V4L2_STD_G		(V4L2_STD_PAL_G		|\
				 V4L2_STD_SECAM_G)
#define V4L2_STD_H		(V4L2_STD_PAL_H		|\
				 V4L2_STD_SECAM_H)
#define V4L2_STD_L		(V4L2_STD_SECAM_L	|\
				 V4L2_STD_SECAM_LC)
#define V4L2_STD_GH		(V4L2_STD_G		|\
				 V4L2_STD_H)
#define V4L2_STD_DK		(V4L2_STD_PAL_DK	|\
				 V4L2_STD_SECAM_DK)
#define V4L2_STD_BG		(V4L2_STD_B		|\
				 V4L2_STD_G)
#define V4L2_STD_MN		(V4L2_STD_PAL_M		|\
				 V4L2_STD_PAL_N		|\
				 V4L2_STD_PAL_Nc	|\
				 V4L2_STD_NTSC)

/* Standards where MTS/BTSC stereo could be found */
#define V4L2_STD_MTS		(V4L2_STD_NTSC_M	|\
				 V4L2_STD_PAL_M		|\
				 V4L2_STD_PAL_N		|\
				 V4L2_STD_PAL_Nc)

/* Standards for Countries with 60Hz Line frequency */
#define V4L2_STD_525_60		(V4L2_STD_PAL_M		|\
				 V4L2_STD_PAL_60	|\
				 V4L2_STD_NTSC		|\
				 V4L2_STD_NTSC_443)
/* Standards for Countries with 50Hz Line frequency */
#define V4L2_STD_625_50		(V4L2_STD_PAL		|\
				 V4L2_STD_PAL_N		|\
				 V4L2_STD_PAL_Nc	|\
				 V4L2_STD_SECAM)

#define V4L2_STD_ATSC           (V4L2_STD_ATSC_8_VSB    |\
				 V4L2_STD_ATSC_16_VSB)
/* Macros with none and all analog standards */
#define V4L2_STD_UNKNOWN        0
#define V4L2_STD_ALL            (V4L2_STD_525_60	|\
				 V4L2_STD_625_50)

struct v4l2_standard {
	__u32		     index;
	v4l2_std_id          id;
	__u8		     name[24];
	struct v4l2_fract    frameperiod; /* Frames, not fields */
	__u32		     framelines;
	__u32		     reserved[4];
};

/*
 *	V I D E O	T I M I N G S	D V	P R E S E T
 */
struct v4l2_dv_preset {
	__u32	preset;
	__u32	reserved[4];
};

/*
 *	D V	P R E S E T S	E N U M E R A T I O N
 */
struct v4l2_dv_enum_preset {
	__u32	index;
	__u32	preset;
	__u8	name[32]; /* Name of the preset timing */
	__u32	width;
	__u32	height;
	__u32	reserved[4];
};

/*
 * 	D V	P R E S E T	V A L U E S
 */
#define		V4L2_DV_INVALID		0
#define		V4L2_DV_480P59_94	1 /* BT.1362 */
#define		V4L2_DV_576P50		2 /* BT.1362 */
#define		V4L2_DV_720P24		3 /* SMPTE 296M */
#define		V4L2_DV_720P25		4 /* SMPTE 296M */
#define		V4L2_DV_720P30		5 /* SMPTE 296M */
#define		V4L2_DV_720P50		6 /* SMPTE 296M */
#define		V4L2_DV_720P59_94	7 /* SMPTE 274M */
#define		V4L2_DV_720P60		8 /* SMPTE 274M/296M */
#define		V4L2_DV_1080I29_97	9 /* BT.1120/ SMPTE 274M */
#define		V4L2_DV_1080I30		10 /* BT.1120/ SMPTE 274M */
#define		V4L2_DV_1080I25		11 /* BT.1120 */
#define		V4L2_DV_1080I50		12 /* SMPTE 296M */
#define		V4L2_DV_1080I60		13 /* SMPTE 296M */
#define		V4L2_DV_1080P24		14 /* SMPTE 296M */
#define		V4L2_DV_1080P25		15 /* SMPTE 296M */
#define		V4L2_DV_1080P30		16 /* SMPTE 296M */
#define		V4L2_DV_1080P50		17 /* BT.1120 */
#define		V4L2_DV_1080P60		18 /* BT.1120 */

/*
 *	D V 	B T	T I M I N G S
 */

/* BT.656/BT.1120 timing data */
struct v4l2_bt_timings {
	__u32	width;		/* width in pixels */
	__u32	height;		/* height in lines */
	__u32	interlaced;	/* Interlaced or progressive */
	__u32	polarities;	/* Positive or negative polarity */
	__u64	pixelclock;	/* Pixel clock in HZ. Ex. 74.25MHz->74250000 */
	__u32	hfrontporch;	/* Horizpontal front porch in pixels */
	__u32	hsync;		/* Horizontal Sync length in pixels */
	__u32	hbackporch;	/* Horizontal back porch in pixels */
	__u32	vfrontporch;	/* Vertical front porch in pixels */
	__u32	vsync;		/* Vertical Sync length in lines */
	__u32	vbackporch;	/* Vertical back porch in lines */
	__u32	il_vfrontporch;	/* Vertical front porch for bottom field of
				 * interlaced field formats
				 */
	__u32	il_vsync;	/* Vertical sync length for bottom field of
				 * interlaced field formats
				 */
	__u32	il_vbackporch;	/* Vertical back porch for bottom field of
				 * interlaced field formats
				 */
	__u32	reserved[16];
} __attribute__ ((packed));

/* Interlaced or progressive format */
#define	V4L2_DV_PROGRESSIVE	0
#define	V4L2_DV_INTERLACED	1

/* Polarities. If bit is not set, it is assumed to be negative polarity */
#define V4L2_DV_VSYNC_POS_POL	0x00000001
#define V4L2_DV_HSYNC_POS_POL	0x00000002


/* DV timings */
struct v4l2_dv_timings {
	__u32 type;
	union {
		struct v4l2_bt_timings	bt;
		__u32	reserved[32];
	};
} __attribute__ ((packed));

/* Values for the type field */
#define V4L2_DV_BT_656_1120	0	/* BT.656/1120 timing type */

/*
 *	V I D E O   I N P U T S
 */
struct v4l2_input {
	__u32	     index;		/*  Which input */
	__u8	     name[32];		/*  Label */
	__u32	     type;		/*  Type of input */
	__u32	     audioset;		/*  Associated audios (bitfield) */
	__u32        tuner;             /*  Associated tuner */
	v4l2_std_id  std;
	__u32	     status;
	__u32	     capabilities;
	__u32	     reserved[3];
};

/*  Values for the 'type' field */
#define V4L2_INPUT_TYPE_TUNER		1
#define V4L2_INPUT_TYPE_CAMERA		2

/* field 'status' - general */
#define V4L2_IN_ST_NO_POWER    0x00000001  /* Attached device is off */
#define V4L2_IN_ST_NO_SIGNAL   0x00000002
#define V4L2_IN_ST_NO_COLOR    0x00000004

/* field 'status' - sensor orientation */
/* If sensor is mounted upside down set both bits */
#define V4L2_IN_ST_HFLIP       0x00000010 /* Frames are flipped horizontally */
#define V4L2_IN_ST_VFLIP       0x00000020 /* Frames are flipped vertically */

/* field 'status' - analog */
#define V4L2_IN_ST_NO_H_LOCK   0x00000100  /* No horizontal sync lock */
#define V4L2_IN_ST_COLOR_KILL  0x00000200  /* Color killer is active */

/* field 'status' - digital */
#define V4L2_IN_ST_NO_SYNC     0x00010000  /* No synchronization lock */
#define V4L2_IN_ST_NO_EQU      0x00020000  /* No equalizer lock */
#define V4L2_IN_ST_NO_CARRIER  0x00040000  /* Carrier recovery failed */

/* field 'status' - VCR and set-top box */
#define V4L2_IN_ST_MACROVISION 0x01000000  /* Macrovision detected */
#define V4L2_IN_ST_NO_ACCESS   0x02000000  /* Conditional access denied */
#define V4L2_IN_ST_VTR         0x04000000  /* VTR time constant */

/* capabilities flags */
#define V4L2_IN_CAP_PRESETS		0x00000001 /* Supports S_DV_PRESET */
#define V4L2_IN_CAP_CUSTOM_TIMINGS	0x00000002 /* Supports S_DV_TIMINGS */
#define V4L2_IN_CAP_STD			0x00000004 /* Supports S_STD */

/*
 *	V I D E O   O U T P U T S
 */
struct v4l2_output {
	__u32	     index;		/*  Which output */
	__u8	     name[32];		/*  Label */
	__u32	     type;		/*  Type of output */
	__u32	     audioset;		/*  Associated audios (bitfield) */
	__u32	     modulator;         /*  Associated modulator */
	v4l2_std_id  std;
	__u32	     capabilities;
	__u32	     reserved[3];
};
/*  Values for the 'type' field */
#define V4L2_OUTPUT_TYPE_MODULATOR		1
#define V4L2_OUTPUT_TYPE_ANALOG			2
#define V4L2_OUTPUT_TYPE_ANALOGVGAOVERLAY	3

/* capabilities flags */
#define V4L2_OUT_CAP_PRESETS		0x00000001 /* Supports S_DV_PRESET */
#define V4L2_OUT_CAP_CUSTOM_TIMINGS	0x00000002 /* Supports S_DV_TIMINGS */
#define V4L2_OUT_CAP_STD		0x00000004 /* Supports S_STD */

/*
 *	C O N T R O L S
 */
struct v4l2_control {
	__u32		     id;
	__s32		     value;
};

struct v4l2_ext_control {
	__u32 id;
	__u32 size;
	__u32 reserved2[1];
	union {
		__s32 value;
		__s64 value64;
		char *string;
	};
} __attribute__ ((packed));

struct v4l2_ext_controls {
	__u32 ctrl_class;
	__u32 count;
	__u32 error_idx;
	__u32 reserved[2];
	struct v4l2_ext_control *controls;
};

/*  Values for ctrl_class field */
#define V4L2_CTRL_CLASS_USER 0x00980000	/* Old-style 'user' controls */
#define V4L2_CTRL_CLASS_MPEG 0x00990000	/* MPEG-compression controls */
#define V4L2_CTRL_CLASS_CAMERA 0x009a0000	/* Camera class controls */
#define V4L2_CTRL_CLASS_FM_TX 0x009b0000	/* FM Modulator control class */
#define V4L2_CTRL_CLASS_FLASH 0x009c0000	/* Camera flash controls */
#define V4L2_CTRL_CLASS_JPEG 0x009d0000		/* JPEG-compression controls */

#define V4L2_CTRL_ID_MASK      	  (0x0fffffff)
#define V4L2_CTRL_ID2CLASS(id)    ((id) & 0x0fff0000UL)
#define V4L2_CTRL_DRIVER_PRIV(id) (((id) & 0xffff) >= 0x1000)

enum v4l2_ctrl_type {
	V4L2_CTRL_TYPE_INTEGER	     = 1,
	V4L2_CTRL_TYPE_BOOLEAN	     = 2,
	V4L2_CTRL_TYPE_MENU	     = 3,
	V4L2_CTRL_TYPE_BUTTON	     = 4,
	V4L2_CTRL_TYPE_INTEGER64     = 5,
	V4L2_CTRL_TYPE_CTRL_CLASS    = 6,
	V4L2_CTRL_TYPE_STRING        = 7,
	V4L2_CTRL_TYPE_BITMASK       = 8,
};

/*  Used in the VIDIOC_QUERYCTRL ioctl for querying controls */
struct v4l2_queryctrl {
	__u32		     id;
	enum v4l2_ctrl_type  type;
	__u8		     name[32];	/* Whatever */
	__s32		     minimum;	/* Note signedness */
	__s32		     maximum;
	__s32		     step;
	__s32		     default_value;
	__u32                flags;
	__u32		     reserved[2];
};

/*  Used in the VIDIOC_QUERYMENU ioctl for querying menu items */
struct v4l2_querymenu {
	__u32		id;
	__u32		index;
	__u8		name[32];	/* Whatever */
	__u32		reserved;
};

/*  Control flags  */
#define V4L2_CTRL_FLAG_DISABLED		0x0001
#define V4L2_CTRL_FLAG_GRABBED		0x0002
#define V4L2_CTRL_FLAG_READ_ONLY 	0x0004
#define V4L2_CTRL_FLAG_UPDATE 		0x0008
#define V4L2_CTRL_FLAG_INACTIVE 	0x0010
#define V4L2_CTRL_FLAG_SLIDER 		0x0020
#define V4L2_CTRL_FLAG_WRITE_ONLY 	0x0040
#define V4L2_CTRL_FLAG_VOLATILE		0x0080

/*  Query flag, to be ORed with the control ID */
#define V4L2_CTRL_FLAG_NEXT_CTRL	0x80000000

/*  User-class control IDs defined by V4L2 */
#define V4L2_CID_MAX_CTRLS		1024
#define V4L2_CID_BASE			(V4L2_CTRL_CLASS_USER | 0x900)
#define V4L2_CID_USER_BASE 		V4L2_CID_BASE
/*  IDs reserved for driver specific controls */
#define V4L2_CID_PRIVATE_BASE		0x08000000

#define V4L2_CID_USER_CLASS 		(V4L2_CTRL_CLASS_USER | 1)
#define V4L2_CID_BRIGHTNESS		(V4L2_CID_BASE+0)
#define V4L2_CID_CONTRAST		(V4L2_CID_BASE+1)
#define V4L2_CID_SATURATION		(V4L2_CID_BASE+2)
#define V4L2_CID_HUE			(V4L2_CID_BASE+3)
#define V4L2_CID_AUDIO_VOLUME		(V4L2_CID_BASE+5)
#define V4L2_CID_AUDIO_BALANCE		(V4L2_CID_BASE+6)
#define V4L2_CID_AUDIO_BASS		(V4L2_CID_BASE+7)
#define V4L2_CID_AUDIO_TREBLE		(V4L2_CID_BASE+8)
#define V4L2_CID_AUDIO_MUTE		(V4L2_CID_BASE+9)
#define V4L2_CID_AUDIO_LOUDNESS		(V4L2_CID_BASE+10)
#define V4L2_CID_BLACK_LEVEL		(V4L2_CID_BASE+11) /* Deprecated */
#define V4L2_CID_AUTO_WHITE_BALANCE	(V4L2_CID_BASE+12)
#define V4L2_CID_DO_WHITE_BALANCE	(V4L2_CID_BASE+13)
#define V4L2_CID_RED_BALANCE		(V4L2_CID_BASE+14)
#define V4L2_CID_BLUE_BALANCE		(V4L2_CID_BASE+15)
#define V4L2_CID_GAMMA			(V4L2_CID_BASE+16)
#define V4L2_CID_WHITENESS		(V4L2_CID_GAMMA) /* Deprecated */
#define V4L2_CID_EXPOSURE		(V4L2_CID_BASE+17)
#define V4L2_CID_AUTOGAIN		(V4L2_CID_BASE+18)
#define V4L2_CID_GAIN			(V4L2_CID_BASE+19)
#define V4L2_CID_HFLIP			(V4L2_CID_BASE+20)
#define V4L2_CID_VFLIP			(V4L2_CID_BASE+21)

/* Deprecated; use V4L2_CID_PAN_RESET and V4L2_CID_TILT_RESET */
#define V4L2_CID_HCENTER		(V4L2_CID_BASE+22)
#define V4L2_CID_VCENTER		(V4L2_CID_BASE+23)

#define V4L2_CID_POWER_LINE_FREQUENCY	(V4L2_CID_BASE+24)
enum v4l2_power_line_frequency {
	V4L2_CID_POWER_LINE_FREQUENCY_DISABLED	= 0,
	V4L2_CID_POWER_LINE_FREQUENCY_50HZ	= 1,
	V4L2_CID_POWER_LINE_FREQUENCY_60HZ	= 2,
	V4L2_CID_POWER_LINE_FREQUENCY_AUTO	= 3,
};
#define V4L2_CID_HUE_AUTO			(V4L2_CID_BASE+25)
#define V4L2_CID_WHITE_BALANCE_TEMPERATURE	(V4L2_CID_BASE+26)
#define V4L2_CID_SHARPNESS			(V4L2_CID_BASE+27)
#define V4L2_CID_BACKLIGHT_COMPENSATION 	(V4L2_CID_BASE+28)
#define V4L2_CID_CHROMA_AGC                     (V4L2_CID_BASE+29)
#define V4L2_CID_COLOR_KILLER                   (V4L2_CID_BASE+30)
#define V4L2_CID_COLORFX			(V4L2_CID_BASE+31)
enum v4l2_colorfx {
	V4L2_COLORFX_NONE	= 0,
	V4L2_COLORFX_BW		= 1,
	V4L2_COLORFX_SEPIA	= 2,
	V4L2_COLORFX_NEGATIVE = 3,
	V4L2_COLORFX_EMBOSS = 4,
	V4L2_COLORFX_SKETCH = 5,
	V4L2_COLORFX_SKY_BLUE = 6,
	V4L2_COLORFX_GRASS_GREEN = 7,
	V4L2_COLORFX_SKIN_WHITEN = 8,
	V4L2_COLORFX_VIVID = 9,
};
#define V4L2_CID_AUTOBRIGHTNESS			(V4L2_CID_BASE+32)
#define V4L2_CID_BAND_STOP_FILTER		(V4L2_CID_BASE+33)

#define V4L2_CID_ROTATE				(V4L2_CID_BASE+34)
#define V4L2_CID_BG_COLOR			(V4L2_CID_BASE+35)

#define V4L2_CID_CHROMA_GAIN                    (V4L2_CID_BASE+36)

#define V4L2_CID_ILLUMINATORS_1			(V4L2_CID_BASE+37)
#define V4L2_CID_ILLUMINATORS_2			(V4L2_CID_BASE+38)

#define V4L2_CID_MIN_BUFFERS_FOR_CAPTURE	(V4L2_CID_BASE+39)
#define V4L2_CID_MIN_BUFFERS_FOR_OUTPUT		(V4L2_CID_BASE+40)

#define V4L2_CID_ALPHA_COMPONENT		(V4L2_CID_BASE+41)

/* last CID + 1 */
#define V4L2_CID_LASTP1                         (V4L2_CID_BASE+42)
#define V4L2_CID_SPECIAL_EFFECT			(V4L2_CID_BASE+43)
/* Minimum number of buffer neede by the device */

/*  MPEG-class control IDs defined by V4L2 */
#define V4L2_CID_MPEG_BASE 			(V4L2_CTRL_CLASS_MPEG | 0x900)
#define V4L2_CID_MPEG_CLASS 			(V4L2_CTRL_CLASS_MPEG | 1)

/*  MPEG streams, specific to multiplexed streams */
#define V4L2_CID_MPEG_STREAM_TYPE 		(V4L2_CID_MPEG_BASE+0)
enum v4l2_mpeg_stream_type {
	V4L2_MPEG_STREAM_TYPE_MPEG2_PS   = 0, /* MPEG-2 program stream */
	V4L2_MPEG_STREAM_TYPE_MPEG2_TS   = 1, /* MPEG-2 transport stream */
	V4L2_MPEG_STREAM_TYPE_MPEG1_SS   = 2, /* MPEG-1 system stream */
	V4L2_MPEG_STREAM_TYPE_MPEG2_DVD  = 3, /* MPEG-2 DVD-compatible stream */
	V4L2_MPEG_STREAM_TYPE_MPEG1_VCD  = 4, /* MPEG-1 VCD-compatible stream */
	V4L2_MPEG_STREAM_TYPE_MPEG2_SVCD = 5, /* MPEG-2 SVCD-compatible stream */
};
#define V4L2_CID_MPEG_STREAM_PID_PMT 		(V4L2_CID_MPEG_BASE+1)
#define V4L2_CID_MPEG_STREAM_PID_AUDIO 		(V4L2_CID_MPEG_BASE+2)
#define V4L2_CID_MPEG_STREAM_PID_VIDEO 		(V4L2_CID_MPEG_BASE+3)
#define V4L2_CID_MPEG_STREAM_PID_PCR 		(V4L2_CID_MPEG_BASE+4)
#define V4L2_CID_MPEG_STREAM_PES_ID_AUDIO 	(V4L2_CID_MPEG_BASE+5)
#define V4L2_CID_MPEG_STREAM_PES_ID_VIDEO 	(V4L2_CID_MPEG_BASE+6)
#define V4L2_CID_MPEG_STREAM_VBI_FMT 		(V4L2_CID_MPEG_BASE+7)
enum v4l2_mpeg_stream_vbi_fmt {
	V4L2_MPEG_STREAM_VBI_FMT_NONE = 0,  /* No VBI in the MPEG stream */
	V4L2_MPEG_STREAM_VBI_FMT_IVTV = 1,  /* VBI in private packets, IVTV format */
};

/*  MPEG audio controls specific to multiplexed streams  */
#define V4L2_CID_MPEG_AUDIO_SAMPLING_FREQ 	(V4L2_CID_MPEG_BASE+100)
enum v4l2_mpeg_audio_sampling_freq {
	V4L2_MPEG_AUDIO_SAMPLING_FREQ_44100 = 0,
	V4L2_MPEG_AUDIO_SAMPLING_FREQ_48000 = 1,
	V4L2_MPEG_AUDIO_SAMPLING_FREQ_32000 = 2,
};
#define V4L2_CID_MPEG_AUDIO_ENCODING 		(V4L2_CID_MPEG_BASE+101)
enum v4l2_mpeg_audio_encoding {
	V4L2_MPEG_AUDIO_ENCODING_LAYER_1 = 0,
	V4L2_MPEG_AUDIO_ENCODING_LAYER_2 = 1,
	V4L2_MPEG_AUDIO_ENCODING_LAYER_3 = 2,
	V4L2_MPEG_AUDIO_ENCODING_AAC     = 3,
	V4L2_MPEG_AUDIO_ENCODING_AC3     = 4,
};
#define V4L2_CID_MPEG_AUDIO_L1_BITRATE 		(V4L2_CID_MPEG_BASE+102)
enum v4l2_mpeg_audio_l1_bitrate {
	V4L2_MPEG_AUDIO_L1_BITRATE_32K  = 0,
	V4L2_MPEG_AUDIO_L1_BITRATE_64K  = 1,
	V4L2_MPEG_AUDIO_L1_BITRATE_96K  = 2,
	V4L2_MPEG_AUDIO_L1_BITRATE_128K = 3,
	V4L2_MPEG_AUDIO_L1_BITRATE_160K = 4,
	V4L2_MPEG_AUDIO_L1_BITRATE_192K = 5,
	V4L2_MPEG_AUDIO_L1_BITRATE_224K = 6,
	V4L2_MPEG_AUDIO_L1_BITRATE_256K = 7,
	V4L2_MPEG_AUDIO_L1_BITRATE_288K = 8,
	V4L2_MPEG_AUDIO_L1_BITRATE_320K = 9,
	V4L2_MPEG_AUDIO_L1_BITRATE_352K = 10,
	V4L2_MPEG_AUDIO_L1_BITRATE_384K = 11,
	V4L2_MPEG_AUDIO_L1_BITRATE_416K = 12,
	V4L2_MPEG_AUDIO_L1_BITRATE_448K = 13,
};
#define V4L2_CID_MPEG_AUDIO_L2_BITRATE 		(V4L2_CID_MPEG_BASE+103)
enum v4l2_mpeg_audio_l2_bitrate {
	V4L2_MPEG_AUDIO_L2_BITRATE_32K  = 0,
	V4L2_MPEG_AUDIO_L2_BITRATE_48K  = 1,
	V4L2_MPEG_AUDIO_L2_BITRATE_56K  = 2,
	V4L2_MPEG_AUDIO_L2_BITRATE_64K  = 3,
	V4L2_MPEG_AUDIO_L2_BITRATE_80K  = 4,
	V4L2_MPEG_AUDIO_L2_BITRATE_96K  = 5,
	V4L2_MPEG_AUDIO_L2_BITRATE_112K = 6,
	V4L2_MPEG_AUDIO_L2_BITRATE_128K = 7,
	V4L2_MPEG_AUDIO_L2_BITRATE_160K = 8,
	V4L2_MPEG_AUDIO_L2_BITRATE_192K = 9,
	V4L2_MPEG_AUDIO_L2_BITRATE_224K = 10,
	V4L2_MPEG_AUDIO_L2_BITRATE_256K = 11,
	V4L2_MPEG_AUDIO_L2_BITRATE_320K = 12,
	V4L2_MPEG_AUDIO_L2_BITRATE_384K = 13,
};
#define V4L2_CID_MPEG_AUDIO_L3_BITRATE 		(V4L2_CID_MPEG_BASE+104)
enum v4l2_mpeg_audio_l3_bitrate {
	V4L2_MPEG_AUDIO_L3_BITRATE_32K  = 0,
	V4L2_MPEG_AUDIO_L3_BITRATE_40K  = 1,
	V4L2_MPEG_AUDIO_L3_BITRATE_48K  = 2,
	V4L2_MPEG_AUDIO_L3_BITRATE_56K  = 3,
	V4L2_MPEG_AUDIO_L3_BITRATE_64K  = 4,
	V4L2_MPEG_AUDIO_L3_BITRATE_80K  = 5,
	V4L2_MPEG_AUDIO_L3_BITRATE_96K  = 6,
	V4L2_MPEG_AUDIO_L3_BITRATE_112K = 7,
	V4L2_MPEG_AUDIO_L3_BITRATE_128K = 8,
	V4L2_MPEG_AUDIO_L3_BITRATE_160K = 9,
	V4L2_MPEG_AUDIO_L3_BITRATE_192K = 10,
	V4L2_MPEG_AUDIO_L3_BITRATE_224K = 11,
	V4L2_MPEG_AUDIO_L3_BITRATE_256K = 12,
	V4L2_MPEG_AUDIO_L3_BITRATE_320K = 13,
};
#define V4L2_CID_MPEG_AUDIO_MODE 		(V4L2_CID_MPEG_BASE+105)
enum v4l2_mpeg_audio_mode {
	V4L2_MPEG_AUDIO_MODE_STEREO       = 0,
	V4L2_MPEG_AUDIO_MODE_JOINT_STEREO = 1,
	V4L2_MPEG_AUDIO_MODE_DUAL         = 2,
	V4L2_MPEG_AUDIO_MODE_MONO         = 3,
};
#define V4L2_CID_MPEG_AUDIO_MODE_EXTENSION 	(V4L2_CID_MPEG_BASE+106)
enum v4l2_mpeg_audio_mode_extension {
	V4L2_MPEG_AUDIO_MODE_EXTENSION_BOUND_4  = 0,
	V4L2_MPEG_AUDIO_MODE_EXTENSION_BOUND_8  = 1,
	V4L2_MPEG_AUDIO_MODE_EXTENSION_BOUND_12 = 2,
	V4L2_MPEG_AUDIO_MODE_EXTENSION_BOUND_16 = 3,
};
#define V4L2_CID_MPEG_AUDIO_EMPHASIS 		(V4L2_CID_MPEG_BASE+107)
enum v4l2_mpeg_audio_emphasis {
	V4L2_MPEG_AUDIO_EMPHASIS_NONE         = 0,
	V4L2_MPEG_AUDIO_EMPHASIS_50_DIV_15_uS = 1,
	V4L2_MPEG_AUDIO_EMPHASIS_CCITT_J17    = 2,
};
#define V4L2_CID_MPEG_AUDIO_CRC 		(V4L2_CID_MPEG_BASE+108)
enum v4l2_mpeg_audio_crc {
	V4L2_MPEG_AUDIO_CRC_NONE  = 0,
	V4L2_MPEG_AUDIO_CRC_CRC16 = 1,
};
#define V4L2_CID_MPEG_AUDIO_MUTE 		(V4L2_CID_MPEG_BASE+109)
#define V4L2_CID_MPEG_AUDIO_AAC_BITRATE		(V4L2_CID_MPEG_BASE+110)
#define V4L2_CID_MPEG_AUDIO_AC3_BITRATE		(V4L2_CID_MPEG_BASE+111)
enum v4l2_mpeg_audio_ac3_bitrate {
	V4L2_MPEG_AUDIO_AC3_BITRATE_32K  = 0,
	V4L2_MPEG_AUDIO_AC3_BITRATE_40K  = 1,
	V4L2_MPEG_AUDIO_AC3_BITRATE_48K  = 2,
	V4L2_MPEG_AUDIO_AC3_BITRATE_56K  = 3,
	V4L2_MPEG_AUDIO_AC3_BITRATE_64K  = 4,
	V4L2_MPEG_AUDIO_AC3_BITRATE_80K  = 5,
	V4L2_MPEG_AUDIO_AC3_BITRATE_96K  = 6,
	V4L2_MPEG_AUDIO_AC3_BITRATE_112K = 7,
	V4L2_MPEG_AUDIO_AC3_BITRATE_128K = 8,
	V4L2_MPEG_AUDIO_AC3_BITRATE_160K = 9,
	V4L2_MPEG_AUDIO_AC3_BITRATE_192K = 10,
	V4L2_MPEG_AUDIO_AC3_BITRATE_224K = 11,
	V4L2_MPEG_AUDIO_AC3_BITRATE_256K = 12,
	V4L2_MPEG_AUDIO_AC3_BITRATE_320K = 13,
	V4L2_MPEG_AUDIO_AC3_BITRATE_384K = 14,
	V4L2_MPEG_AUDIO_AC3_BITRATE_448K = 15,
	V4L2_MPEG_AUDIO_AC3_BITRATE_512K = 16,
	V4L2_MPEG_AUDIO_AC3_BITRATE_576K = 17,
	V4L2_MPEG_AUDIO_AC3_BITRATE_640K = 18,
};
#define V4L2_CID_MPEG_AUDIO_DEC_PLAYBACK	(V4L2_CID_MPEG_BASE+112)
enum v4l2_mpeg_audio_dec_playback {
	V4L2_MPEG_AUDIO_DEC_PLAYBACK_AUTO	    = 0,
	V4L2_MPEG_AUDIO_DEC_PLAYBACK_STEREO	    = 1,
	V4L2_MPEG_AUDIO_DEC_PLAYBACK_LEFT	    = 2,
	V4L2_MPEG_AUDIO_DEC_PLAYBACK_RIGHT	    = 3,
	V4L2_MPEG_AUDIO_DEC_PLAYBACK_MONO	    = 4,
	V4L2_MPEG_AUDIO_DEC_PLAYBACK_SWAPPED_STEREO = 5,
};
#define V4L2_CID_MPEG_AUDIO_DEC_MULTILINGUAL_PLAYBACK (V4L2_CID_MPEG_BASE+113)

/*  MPEG video controls specific to multiplexed streams */
#define V4L2_CID_MPEG_VIDEO_ENCODING 		(V4L2_CID_MPEG_BASE+200)
enum v4l2_mpeg_video_encoding {
	V4L2_MPEG_VIDEO_ENCODING_MPEG_1     = 0,
	V4L2_MPEG_VIDEO_ENCODING_MPEG_2     = 1,
	V4L2_MPEG_VIDEO_ENCODING_MPEG_4_AVC = 2,
};
#define V4L2_CID_MPEG_VIDEO_ASPECT 		(V4L2_CID_MPEG_BASE+201)
enum v4l2_mpeg_video_aspect {
	V4L2_MPEG_VIDEO_ASPECT_1x1     = 0,
	V4L2_MPEG_VIDEO_ASPECT_4x3     = 1,
	V4L2_MPEG_VIDEO_ASPECT_16x9    = 2,
	V4L2_MPEG_VIDEO_ASPECT_221x100 = 3,
};
#define V4L2_CID_MPEG_VIDEO_B_FRAMES 		(V4L2_CID_MPEG_BASE+202)
#define V4L2_CID_MPEG_VIDEO_GOP_SIZE 		(V4L2_CID_MPEG_BASE+203)
#define V4L2_CID_MPEG_VIDEO_GOP_CLOSURE 	(V4L2_CID_MPEG_BASE+204)
#define V4L2_CID_MPEG_VIDEO_PULLDOWN 		(V4L2_CID_MPEG_BASE+205)
#define V4L2_CID_MPEG_VIDEO_BITRATE_MODE 	(V4L2_CID_MPEG_BASE+206)
enum v4l2_mpeg_video_bitrate_mode {
	V4L2_MPEG_VIDEO_BITRATE_MODE_VBR = 0,
	V4L2_MPEG_VIDEO_BITRATE_MODE_CBR = 1,
};
#define V4L2_CID_MPEG_VIDEO_BITRATE 		(V4L2_CID_MPEG_BASE+207)
#define V4L2_CID_MPEG_VIDEO_BITRATE_PEAK 	(V4L2_CID_MPEG_BASE+208)
#define V4L2_CID_MPEG_VIDEO_TEMPORAL_DECIMATION (V4L2_CID_MPEG_BASE+209)
#define V4L2_CID_MPEG_VIDEO_MUTE 		(V4L2_CID_MPEG_BASE+210)
#define V4L2_CID_MPEG_VIDEO_MUTE_YUV 		(V4L2_CID_MPEG_BASE+211)
#define V4L2_CID_MPEG_VIDEO_DECODER_SLICE_INTERFACE		(V4L2_CID_MPEG_BASE+212)
#define V4L2_CID_MPEG_VIDEO_DECODER_MPEG4_DEBLOCK_FILTER	(V4L2_CID_MPEG_BASE+213)
#define V4L2_CID_MPEG_VIDEO_CYCLIC_INTRA_REFRESH_MB		(V4L2_CID_MPEG_BASE+214)
#define V4L2_CID_MPEG_VIDEO_FRAME_RC_ENABLE			(V4L2_CID_MPEG_BASE+215)
#define V4L2_CID_MPEG_VIDEO_HEADER_MODE				(V4L2_CID_MPEG_BASE+216)
enum v4l2_mpeg_video_header_mode {
	V4L2_MPEG_VIDEO_HEADER_MODE_SEPARATE			= 0,
	V4L2_MPEG_VIDEO_HEADER_MODE_JOINED_WITH_1ST_FRAME	= 1,
	V4L2_MPEG_VIDEO_HEADER_MODE_JOINED_WITH_I_FRAME		= 2,

};
#define V4L2_CID_MPEG_VIDEO_MAX_REF_PIC			(V4L2_CID_MPEG_BASE+217)
#define V4L2_CID_MPEG_VIDEO_MB_RC_ENABLE		(V4L2_CID_MPEG_BASE+218)
#define V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_BYTES	(V4L2_CID_MPEG_BASE+219)
#define V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MAX_MB		(V4L2_CID_MPEG_BASE+220)
#define V4L2_CID_MPEG_VIDEO_MULTI_SLICE_MODE		(V4L2_CID_MPEG_BASE+221)
enum v4l2_mpeg_video_multi_slice_mode {
	V4L2_MPEG_VIDEO_MULTI_SLICE_MODE_SINGLE		= 0,
	V4L2_MPEG_VIDEO_MULTI_SICE_MODE_MAX_MB		= 1,
	V4L2_MPEG_VIDEO_MULTI_SICE_MODE_MAX_BYTES	= 2,
	V4L2_MPEG_VIDEO_MULTI_SLICE_GOB			= 3,
};
#define V4L2_CID_MPEG_VIDEO_VBV_SIZE			(V4L2_CID_MPEG_BASE+222)
#define V4L2_CID_MPEG_VIDEO_DEC_PTS			(V4L2_CID_MPEG_BASE+223)
#define V4L2_CID_MPEG_VIDEO_DEC_FRAME			(V4L2_CID_MPEG_BASE+224)

#define V4L2_CID_MPEG_VIDEO_H263_I_FRAME_QP		(V4L2_CID_MPEG_BASE+300)
#define V4L2_CID_MPEG_VIDEO_H263_P_FRAME_QP		(V4L2_CID_MPEG_BASE+301)
#define V4L2_CID_MPEG_VIDEO_H263_B_FRAME_QP		(V4L2_CID_MPEG_BASE+302)
#define V4L2_CID_MPEG_VIDEO_H263_MIN_QP			(V4L2_CID_MPEG_BASE+303)
#define V4L2_CID_MPEG_VIDEO_H263_MAX_QP			(V4L2_CID_MPEG_BASE+304)
#define V4L2_CID_MPEG_VIDEO_H264_I_FRAME_QP		(V4L2_CID_MPEG_BASE+350)
#define V4L2_CID_MPEG_VIDEO_H264_P_FRAME_QP		(V4L2_CID_MPEG_BASE+351)
#define V4L2_CID_MPEG_VIDEO_H264_B_FRAME_QP		(V4L2_CID_MPEG_BASE+352)
#define V4L2_CID_MPEG_VIDEO_H264_MIN_QP			(V4L2_CID_MPEG_BASE+353)
#define V4L2_CID_MPEG_VIDEO_H264_MAX_QP			(V4L2_CID_MPEG_BASE+354)
#define V4L2_CID_MPEG_VIDEO_H264_8X8_TRANSFORM		(V4L2_CID_MPEG_BASE+355)
#define V4L2_CID_MPEG_VIDEO_H264_CPB_SIZE		(V4L2_CID_MPEG_BASE+356)
#define V4L2_CID_MPEG_VIDEO_H264_ENTROPY_MODE		(V4L2_CID_MPEG_BASE+357)
enum v4l2_mpeg_video_h264_entropy_mode {
	V4L2_MPEG_VIDEO_H264_ENTROPY_MODE_CAVLC	= 0,
	V4L2_MPEG_VIDEO_H264_ENTROPY_MODE_CABAC	= 1,
};
#define V4L2_CID_MPEG_VIDEO_H264_I_PERIOD		(V4L2_CID_MPEG_BASE+358)
#define V4L2_CID_MPEG_VIDEO_H264_LEVEL			(V4L2_CID_MPEG_BASE+359)
enum v4l2_mpeg_video_h264_level {
	V4L2_MPEG_VIDEO_H264_LEVEL_1_0	= 0,
	V4L2_MPEG_VIDEO_H264_LEVEL_1B	= 1,
	V4L2_MPEG_VIDEO_H264_LEVEL_1_1	= 2,
	V4L2_MPEG_VIDEO_H264_LEVEL_1_2	= 3,
	V4L2_MPEG_VIDEO_H264_LEVEL_1_3	= 4,
	V4L2_MPEG_VIDEO_H264_LEVEL_2_0	= 5,
	V4L2_MPEG_VIDEO_H264_LEVEL_2_1	= 6,
	V4L2_MPEG_VIDEO_H264_LEVEL_2_2	= 7,
	V4L2_MPEG_VIDEO_H264_LEVEL_3_0	= 8,
	V4L2_MPEG_VIDEO_H264_LEVEL_3_1	= 9,
	V4L2_MPEG_VIDEO_H264_LEVEL_3_2	= 10,
	V4L2_MPEG_VIDEO_H264_LEVEL_4_0	= 11,
	V4L2_MPEG_VIDEO_H264_LEVEL_4_1	= 12,
	V4L2_MPEG_VIDEO_H264_LEVEL_4_2	= 13,
	V4L2_MPEG_VIDEO_H264_LEVEL_5_0	= 14,
	V4L2_MPEG_VIDEO_H264_LEVEL_5_1	= 15,
	V4L2_MPEG_VIDEO_H264_LEVEL_5_2	= 16,
};
#define V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_ALPHA	(V4L2_CID_MPEG_BASE+360)
#define V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_BETA	(V4L2_CID_MPEG_BASE+361)
#define V4L2_CID_MPEG_VIDEO_H264_LOOP_FILTER_MODE	(V4L2_CID_MPEG_BASE+362)
enum v4l2_mpeg_video_h264_loop_filter_mode {
	V4L2_MPEG_VIDEO_H264_LOOP_FILTER_MODE_ENABLED				= 0,
	V4L2_MPEG_VIDEO_H264_LOOP_FILTER_MODE_DISABLED				= 1,
	V4L2_MPEG_VIDEO_H264_LOOP_FILTER_MODE_DISABLED_AT_SLICE_BOUNDARY	= 2,
};
#define V4L2_CID_MPEG_VIDEO_H264_PROFILE		(V4L2_CID_MPEG_BASE+363)
enum v4l2_mpeg_video_h264_profile {
	V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE			= 0,
	V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE	= 1,
	V4L2_MPEG_VIDEO_H264_PROFILE_MAIN			= 2,
	V4L2_MPEG_VIDEO_H264_PROFILE_EXTENDED			= 3,
	V4L2_MPEG_VIDEO_H264_PROFILE_HIGH			= 4,
	V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_10			= 5,
	V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_422			= 6,
	V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_444_PREDICTIVE	= 7,
	V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_10_INTRA		= 8,
	V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_422_INTRA		= 9,
	V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_444_INTRA		= 10,
	V4L2_MPEG_VIDEO_H264_PROFILE_CAVLC_444_INTRA		= 11,
	V4L2_MPEG_VIDEO_H264_PROFILE_SCALABLE_BASELINE		= 12,
	V4L2_MPEG_VIDEO_H264_PROFILE_SCALABLE_HIGH		= 13,
	V4L2_MPEG_VIDEO_H264_PROFILE_SCALABLE_HIGH_INTRA	= 14,
	V4L2_MPEG_VIDEO_H264_PROFILE_STEREO_HIGH		= 15,
	V4L2_MPEG_VIDEO_H264_PROFILE_MULTIVIEW_HIGH		= 16,
	V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_HIGH		= 17,
};
#define V4L2_CID_MPEG_VIDEO_H264_VUI_EXT_SAR_HEIGHT	(V4L2_CID_MPEG_BASE+364)
#define V4L2_CID_MPEG_VIDEO_H264_VUI_EXT_SAR_WIDTH	(V4L2_CID_MPEG_BASE+365)
#define V4L2_CID_MPEG_VIDEO_H264_VUI_SAR_ENABLE		(V4L2_CID_MPEG_BASE+366)
#define V4L2_CID_MPEG_VIDEO_H264_VUI_SAR_IDC		(V4L2_CID_MPEG_BASE+367)
enum v4l2_mpeg_video_h264_vui_sar_idc {
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_UNSPECIFIED	= 0,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_1x1		= 1,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_12x11		= 2,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_10x11		= 3,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_16x11		= 4,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_40x33		= 5,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_24x11		= 6,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_20x11		= 7,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_32x11		= 8,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_80x33		= 9,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_18x11		= 10,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_15x11		= 11,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_64x33		= 12,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_160x99		= 13,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_4x3		= 14,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_3x2		= 15,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_2x1		= 16,
	V4L2_MPEG_VIDEO_H264_VUI_SAR_IDC_EXTENDED	= 17,
};
#define V4L2_CID_MPEG_VIDEO_MPEG4_I_FRAME_QP	(V4L2_CID_MPEG_BASE+400)
#define V4L2_CID_MPEG_VIDEO_MPEG4_P_FRAME_QP	(V4L2_CID_MPEG_BASE+401)
#define V4L2_CID_MPEG_VIDEO_MPEG4_B_FRAME_QP	(V4L2_CID_MPEG_BASE+402)
#define V4L2_CID_MPEG_VIDEO_MPEG4_MIN_QP	(V4L2_CID_MPEG_BASE+403)
#define V4L2_CID_MPEG_VIDEO_MPEG4_MAX_QP	(V4L2_CID_MPEG_BASE+404)
#define V4L2_CID_MPEG_VIDEO_MPEG4_LEVEL		(V4L2_CID_MPEG_BASE+405)
enum v4l2_mpeg_video_mpeg4_level {
	V4L2_MPEG_VIDEO_MPEG4_LEVEL_0	= 0,
	V4L2_MPEG_VIDEO_MPEG4_LEVEL_0B	= 1,
	V4L2_MPEG_VIDEO_MPEG4_LEVEL_1	= 2,
	V4L2_MPEG_VIDEO_MPEG4_LEVEL_2	= 3,
	V4L2_MPEG_VIDEO_MPEG4_LEVEL_3	= 4,
	V4L2_MPEG_VIDEO_MPEG4_LEVEL_3B	= 5,
	V4L2_MPEG_VIDEO_MPEG4_LEVEL_4	= 6,
	V4L2_MPEG_VIDEO_MPEG4_LEVEL_5	= 7,
};
#define V4L2_CID_MPEG_VIDEO_MPEG4_PROFILE	(V4L2_CID_MPEG_BASE+406)
enum v4l2_mpeg_video_mpeg4_profile {
	V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE				= 0,
	V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_SIMPLE			= 1,
	V4L2_MPEG_VIDEO_MPEG4_PROFILE_CORE				= 2,
	V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE_SCALABLE			= 3,
	V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_CODING_EFFICIENCY	= 4,
};
#define V4L2_CID_MPEG_VIDEO_MPEG4_QPEL		(V4L2_CID_MPEG_BASE+407)
#define V4L2_CID_QCOM_VIDEO_SYNC_FRAME_SEQ_HDR		(V4L2_CID_MPEG_BASE+408)

/*  MPEG-class control IDs specific to the CX2341x driver as defined by V4L2 */
#define V4L2_CID_MPEG_CX2341X_BASE 				(V4L2_CTRL_CLASS_MPEG | 0x1000)
#define V4L2_CID_MPEG_CX2341X_VIDEO_SPATIAL_FILTER_MODE 	(V4L2_CID_MPEG_CX2341X_BASE+0)
enum v4l2_mpeg_cx2341x_video_spatial_filter_mode {
	V4L2_MPEG_CX2341X_VIDEO_SPATIAL_FILTER_MODE_MANUAL = 0,
	V4L2_MPEG_CX2341X_VIDEO_SPATIAL_FILTER_MODE_AUTO   = 1,
};
#define V4L2_CID_MPEG_CX2341X_VIDEO_SPATIAL_FILTER 		(V4L2_CID_MPEG_CX2341X_BASE+1)
#define V4L2_CID_MPEG_CX2341X_VIDEO_LUMA_SPATIAL_FILTER_TYPE 	(V4L2_CID_MPEG_CX2341X_BASE+2)
enum v4l2_mpeg_cx2341x_video_luma_spatial_filter_type {
	V4L2_MPEG_CX2341X_VIDEO_LUMA_SPATIAL_FILTER_TYPE_OFF                  = 0,
	V4L2_MPEG_CX2341X_VIDEO_LUMA_SPATIAL_FILTER_TYPE_1D_HOR               = 1,
	V4L2_MPEG_CX2341X_VIDEO_LUMA_SPATIAL_FILTER_TYPE_1D_VERT              = 2,
	V4L2_MPEG_CX2341X_VIDEO_LUMA_SPATIAL_FILTER_TYPE_2D_HV_SEPARABLE      = 3,
	V4L2_MPEG_CX2341X_VIDEO_LUMA_SPATIAL_FILTER_TYPE_2D_SYM_NON_SEPARABLE = 4,
};
#define V4L2_CID_MPEG_CX2341X_VIDEO_CHROMA_SPATIAL_FILTER_TYPE 	(V4L2_CID_MPEG_CX2341X_BASE+3)
enum v4l2_mpeg_cx2341x_video_chroma_spatial_filter_type {
	V4L2_MPEG_CX2341X_VIDEO_CHROMA_SPATIAL_FILTER_TYPE_OFF    = 0,
	V4L2_MPEG_CX2341X_VIDEO_CHROMA_SPATIAL_FILTER_TYPE_1D_HOR = 1,
};
#define V4L2_CID_MPEG_CX2341X_VIDEO_TEMPORAL_FILTER_MODE 	(V4L2_CID_MPEG_CX2341X_BASE+4)
enum v4l2_mpeg_cx2341x_video_temporal_filter_mode {
	V4L2_MPEG_CX2341X_VIDEO_TEMPORAL_FILTER_MODE_MANUAL = 0,
	V4L2_MPEG_CX2341X_VIDEO_TEMPORAL_FILTER_MODE_AUTO   = 1,
};
#define V4L2_CID_MPEG_CX2341X_VIDEO_TEMPORAL_FILTER 		(V4L2_CID_MPEG_CX2341X_BASE+5)
#define V4L2_CID_MPEG_CX2341X_VIDEO_MEDIAN_FILTER_TYPE 		(V4L2_CID_MPEG_CX2341X_BASE+6)
enum v4l2_mpeg_cx2341x_video_median_filter_type {
	V4L2_MPEG_CX2341X_VIDEO_MEDIAN_FILTER_TYPE_OFF      = 0,
	V4L2_MPEG_CX2341X_VIDEO_MEDIAN_FILTER_TYPE_HOR      = 1,
	V4L2_MPEG_CX2341X_VIDEO_MEDIAN_FILTER_TYPE_VERT     = 2,
	V4L2_MPEG_CX2341X_VIDEO_MEDIAN_FILTER_TYPE_HOR_VERT = 3,
	V4L2_MPEG_CX2341X_VIDEO_MEDIAN_FILTER_TYPE_DIAG     = 4,
};
#define V4L2_CID_MPEG_CX2341X_VIDEO_LUMA_MEDIAN_FILTER_BOTTOM 	(V4L2_CID_MPEG_CX2341X_BASE+7)
#define V4L2_CID_MPEG_CX2341X_VIDEO_LUMA_MEDIAN_FILTER_TOP 	(V4L2_CID_MPEG_CX2341X_BASE+8)
#define V4L2_CID_MPEG_CX2341X_VIDEO_CHROMA_MEDIAN_FILTER_BOTTOM	(V4L2_CID_MPEG_CX2341X_BASE+9)
#define V4L2_CID_MPEG_CX2341X_VIDEO_CHROMA_MEDIAN_FILTER_TOP 	(V4L2_CID_MPEG_CX2341X_BASE+10)
#define V4L2_CID_MPEG_CX2341X_STREAM_INSERT_NAV_PACKETS 	(V4L2_CID_MPEG_CX2341X_BASE+11)

/*  MPEG-class control IDs specific to the Samsung MFC 5.1 driver as defined by V4L2 */
#define V4L2_CID_MPEG_MFC51_BASE				(V4L2_CTRL_CLASS_MPEG | 0x1100)

#define V4L2_CID_MPEG_MFC51_VIDEO_DECODER_H264_DISPLAY_DELAY		(V4L2_CID_MPEG_MFC51_BASE+0)
#define V4L2_CID_MPEG_MFC51_VIDEO_DECODER_H264_DISPLAY_DELAY_ENABLE	(V4L2_CID_MPEG_MFC51_BASE+1)
#define V4L2_CID_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE			(V4L2_CID_MPEG_MFC51_BASE+2)
enum v4l2_mpeg_mfc51_video_frame_skip_mode {
	V4L2_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE_DISABLED		= 0,
	V4L2_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE_LEVEL_LIMIT	= 1,
	V4L2_MPEG_MFC51_VIDEO_FRAME_SKIP_MODE_BUF_LIMIT		= 2,
};
#define V4L2_CID_MPEG_MFC51_VIDEO_FORCE_FRAME_TYPE			(V4L2_CID_MPEG_MFC51_BASE+3)
enum v4l2_mpeg_mfc51_video_force_frame_type {
	V4L2_MPEG_MFC51_VIDEO_FORCE_FRAME_TYPE_DISABLED		= 0,
	V4L2_MPEG_MFC51_VIDEO_FORCE_FRAME_TYPE_I_FRAME		= 1,
	V4L2_MPEG_MFC51_VIDEO_FORCE_FRAME_TYPE_NOT_CODED	= 2,
};
#define V4L2_CID_MPEG_MFC51_VIDEO_PADDING				(V4L2_CID_MPEG_MFC51_BASE+4)
#define V4L2_CID_MPEG_MFC51_VIDEO_PADDING_YUV				(V4L2_CID_MPEG_MFC51_BASE+5)
#define V4L2_CID_MPEG_MFC51_VIDEO_RC_FIXED_TARGET_BIT			(V4L2_CID_MPEG_MFC51_BASE+6)
#define V4L2_CID_MPEG_MFC51_VIDEO_RC_REACTION_COEFF			(V4L2_CID_MPEG_MFC51_BASE+7)
#define V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_ACTIVITY		(V4L2_CID_MPEG_MFC51_BASE+50)
#define V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_DARK			(V4L2_CID_MPEG_MFC51_BASE+51)
#define V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_SMOOTH		(V4L2_CID_MPEG_MFC51_BASE+52)
#define V4L2_CID_MPEG_MFC51_VIDEO_H264_ADAPTIVE_RC_STATIC		(V4L2_CID_MPEG_MFC51_BASE+53)
#define V4L2_CID_MPEG_MFC51_VIDEO_H264_NUM_REF_PIC_FOR_P		(V4L2_CID_MPEG_MFC51_BASE+54)

/*  MPEG-class control IDs specific to the msm_vidc driver */
#define V4L2_CID_MPEG_MSM_VIDC_BASE		(V4L2_CTRL_CLASS_MPEG | 0x2000)

#define V4L2_CID_MPEG_VIDC_VIDEO_ENABLE_PICTURE_TYPE \
			(V4L2_CID_MPEG_MSM_VIDC_BASE+0)
#define V4L2_CID_MPEG_VIDC_VIDEO_KEEP_ASPECT_RATIO \
			(V4L2_CID_MPEG_MSM_VIDC_BASE+1)
#define V4L2_CID_MPEG_VIDC_VIDEO_POST_LOOP_DEBLOCKER_MODE \
			(V4L2_CID_MPEG_MSM_VIDC_BASE+2)
#define V4L2_CID_MPEG_VIDC_VIDEO_DIVX_FORMAT \
			(V4L2_CID_MPEG_MSM_VIDC_BASE+3)
enum v4l2_mpeg_vidc_video_divx_format_type {
	V4L2_MPEG_VIDC_VIDEO_DIVX_FORMAT_4		= 0,
	V4L2_MPEG_VIDC_VIDEO_DIVX_FORMAT_5		= 1,
	V4L2_MPEG_VIDC_VIDEO_DIVX_FORMAT_6	    = 2,
};
#define V4L2_CID_MPEG_VIDC_VIDEO_MB_ERROR_MAP_REPORTING	\
			(V4L2_CID_MPEG_MSM_VIDC_BASE+4)
#define V4L2_CID_MPEG_VIDC_VIDEO_CONTINUE_DATA_TRANSFER \
			(V4L2_CID_MPEG_MSM_VIDC_BASE+5)

#define V4L2_CID_MPEG_VIDC_VIDEO_STREAM_FORMAT   (V4L2_CID_MPEG_MSM_VIDC_BASE+6)
enum v4l2_mpeg_vidc_video_stream_format {
	V4L2_MPEG_VIDC_VIDEO_NAL_FORMAT_STARTCODES         = 0,
	V4L2_MPEG_VIDC_VIDEO_NAL_FORMAT_ONE_NAL_PER_BUFFER = 1,
	V4L2_MPEG_VIDC_VIDEO_NAL_FORMAT_ONE_BYTE_LENGTH    = 2,
	V4L2_MPEG_VIDC_VIDEO_NAL_FORMAT_TWO_BYTE_LENGTH    = 3,
	V4L2_MPEG_VIDC_VIDEO_NAL_FORMAT_FOUR_BYTE_LENGTH   = 4,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_OUTPUT_ORDER   (V4L2_CID_MPEG_MSM_VIDC_BASE+7)
enum v4l2_mpeg_vidc_video_output_order {
	V4L2_MPEG_VIDC_VIDEO_OUTPUT_ORDER_DISPLAY         = 0,
	V4L2_MPEG_VIDC_VIDEO_OUTPUT_ORDER_DECODE          = 1,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_FRAME_RATE   (V4L2_CID_MPEG_MSM_VIDC_BASE+8)
#define V4L2_CID_MPEG_VIDC_VIDEO_IDR_PERIOD   (V4L2_CID_MPEG_MSM_VIDC_BASE+9)
#define V4L2_CID_MPEG_VIDC_VIDEO_NUM_P_FRAMES (V4L2_CID_MPEG_MSM_VIDC_BASE+10)
#define V4L2_CID_MPEG_VIDC_VIDEO_NUM_B_FRAMES (V4L2_CID_MPEG_MSM_VIDC_BASE+11)
#define V4L2_CID_MPEG_VIDC_VIDEO_REQUEST_IFRAME (V4L2_CID_MPEG_MSM_VIDC_BASE+12)

#define V4L2_CID_MPEG_VIDC_VIDEO_RATE_CONTROL (V4L2_CID_MPEG_MSM_VIDC_BASE+13)
enum v4l2_mpeg_vidc_video_rate_control {
	V4L2_CID_MPEG_VIDC_VIDEO_RATE_CONTROL_OFF = 0,
	V4L2_CID_MPEG_VIDC_VIDEO_RATE_CONTROL_VBR_VFR = 1,
	V4L2_CID_MPEG_VIDC_VIDEO_RATE_CONTROL_VBR_CFR = 2,
	V4L2_CID_MPEG_VIDC_VIDEO_RATE_CONTROL_CBR_VFR = 3,
	V4L2_CID_MPEG_VIDC_VIDEO_RATE_CONTROL_CBR_CFR = 4,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_ROTATION (V4L2_CID_MPEG_MSM_VIDC_BASE+14)
enum v4l2_mpeg_vidc_video_rotation {
	V4L2_CID_MPEG_VIDC_VIDEO_ROTATION_NONE = 0,
	V4L2_CID_MPEG_VIDC_VIDEO_ROTATION_90 = 1,
	V4L2_CID_MPEG_VIDC_VIDEO_ROTATION_180 = 2,
	V4L2_CID_MPEG_VIDC_VIDEO_ROTATION_270 = 3,
};
#define MSM_VIDC_BASE V4L2_CID_MPEG_MSM_VIDC_BASE
#define V4L2_CID_MPEG_VIDC_VIDEO_H264_CABAC_MODEL (MSM_VIDC_BASE+15)
enum v4l2_mpeg_vidc_h264_cabac_model {
	V4L2_CID_MPEG_VIDC_VIDEO_H264_CABAC_MODEL_0 = 0,
	V4L2_CID_MPEG_VIDC_VIDEO_H264_CABAC_MODEL_1 = 1,
	V4L2_CID_MPEG_VIDC_VIDEO_H264_CABAC_MODEL_2 = 2,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_MODE (MSM_VIDC_BASE+16)
enum v4l2_mpeg_vidc_video_intra_refresh_mode {
	V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_NONE = 0,
	V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_CYCLIC = 1,
	V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_ADAPTIVE = 2,
	V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_CYCLIC_ADAPTIVE = 3,
	V4L2_CID_MPEG_VIDC_VIDEO_INTRA_REFRESH_RANDOM = 4,
};
#define V4L2_CID_MPEG_VIDC_VIDEO_AIR_MBS (V4L2_CID_MPEG_MSM_VIDC_BASE+17)
#define V4L2_CID_MPEG_VIDC_VIDEO_AIR_REF (V4L2_CID_MPEG_MSM_VIDC_BASE+18)
#define V4L2_CID_MPEG_VIDC_VIDEO_CIR_MBS (V4L2_CID_MPEG_MSM_VIDC_BASE+19)

#define V4L2_CID_MPEG_VIDC_VIDEO_H263_PROFILE (V4L2_CID_MPEG_MSM_VIDC_BASE+20)
enum v4l2_mpeg_vidc_video_h263_profile {
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_BASELINE = 0,
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_H320CODING	= 1,
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_BACKWARDCOMPATIBLE = 2,
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_ISWV2 = 3,
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_ISWV3 = 4,
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_HIGHCOMPRESSION = 5,
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_INTERNET = 6,
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_INTERLACE = 7,
	V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_HIGHLATENCY = 8,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_H263_LEVEL (V4L2_CID_MPEG_MSM_VIDC_BASE+21)
enum v4l2_mpeg_vidc_video_h263_level {
	V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_1_0 = 0,
	V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_2_0 = 1,
	V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_3_0 = 2,
	V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_4_0 = 3,
	V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_4_5 = 4,
	V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_5_0 = 5,
	V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_6_0 = 6,
	V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_7_0 = 7,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_H264_AU_DELIMITER \
		(V4L2_CID_MPEG_MSM_VIDC_BASE + 22)
enum v4l2_mpeg_vidc_video_h264_au_delimiter {
	V4L2_MPEG_VIDC_VIDEO_H264_AU_DELIMITER_DISABLED = 0,
	V4L2_MPEG_VIDC_VIDEO_H264_AU_DELIMITER_ENABLED = 1
};
#define V4L2_CID_MPEG_VIDC_VIDEO_SYNC_FRAME_DECODE \
		(V4L2_CID_MPEG_MSM_VIDC_BASE + 23)
enum v4l2_mpeg_vidc_video_sync_frame_decode {
	V4L2_MPEG_VIDC_VIDEO_SYNC_FRAME_DECODE_DISABLE = 0,
	V4L2_MPEG_VIDC_VIDEO_SYNC_FRAME_DECODE_ENABLE = 1
};
#define V4L2_CID_MPEG_VIDC_VIDEO_SECURE (V4L2_CID_MPEG_MSM_VIDC_BASE+24)
#define V4L2_CID_MPEG_VIDC_VIDEO_EXTRADATA \
		(V4L2_CID_MPEG_MSM_VIDC_BASE + 25)
enum v4l2_mpeg_vidc_extradata {
	V4L2_MPEG_VIDC_EXTRADATA_NONE,
	V4L2_MPEG_VIDC_EXTRADATA_MB_QUANTIZATION,
	V4L2_MPEG_VIDC_EXTRADATA_INTERLACE_VIDEO,
	V4L2_MPEG_VIDC_EXTRADATA_VC1_FRAMEDISP,
	V4L2_MPEG_VIDC_EXTRADATA_VC1_SEQDISP,
	V4L2_MPEG_VIDC_EXTRADATA_TIMESTAMP,
	V4L2_MPEG_VIDC_EXTRADATA_S3D_FRAME_PACKING,
	V4L2_MPEG_VIDC_EXTRADATA_FRAME_RATE,
	V4L2_MPEG_VIDC_EXTRADATA_PANSCAN_WINDOW,
	V4L2_MPEG_VIDC_EXTRADATA_RECOVERY_POINT_SEI,
	V4L2_MPEG_VIDC_EXTRADATA_CLOSED_CAPTION_UD,
	V4L2_MPEG_VIDC_EXTRADATA_AFD_UD,
	V4L2_MPEG_VIDC_EXTRADATA_MULTISLICE_INFO,
	V4L2_MPEG_VIDC_EXTRADATA_NUM_CONCEALED_MB,
	V4L2_MPEG_VIDC_EXTRADATA_METADATA_FILLER,
	V4L2_MPEG_VIDC_INDEX_EXTRADATA_INPUT_CROP,
	V4L2_MPEG_VIDC_INDEX_EXTRADATA_DIGITAL_ZOOM,
	V4L2_MPEG_VIDC_INDEX_EXTRADATA_ASPECT_RATIO,
	V4L2_MPEG_VIDC_EXTRADATA_MPEG2_SEQDISP
};

#define V4L2_CID_MPEG_VIDC_SET_PERF_LEVEL (V4L2_CID_MPEG_MSM_VIDC_BASE + 26)
enum v4l2_mpeg_vidc_perf_level {
	V4L2_CID_MPEG_VIDC_PERF_LEVEL_NOMINAL			= 0,
	V4L2_CID_MPEG_VIDC_PERF_LEVEL_PERFORMANCE		= 1,
	V4L2_CID_MPEG_VIDC_PERF_LEVEL_TURBO			= 2,
};

#define V4L2_CID_MPEG_VIDEO_MULTI_SLICE_GOB		\
		(V4L2_CID_MPEG_MSM_VIDC_BASE + 27)

#define V4L2_CID_MPEG_VIDEO_MULTI_SLICE_DELIVERY_MODE	\
	(V4L2_CID_MPEG_MSM_VIDC_BASE + 28)

#define V4L2_CID_MPEG_VIDC_VIDEO_H264_VUI_TIMING_INFO \
		(V4L2_CID_MPEG_MSM_VIDC_BASE + 29)
enum v4l2_mpeg_vidc_video_h264_vui_timing_info {
	V4L2_MPEG_VIDC_VIDEO_H264_VUI_TIMING_INFO_DISABLED = 0,
	V4L2_MPEG_VIDC_VIDEO_H264_VUI_TIMING_INFO_ENABLED = 1
};

#define V4L2_CID_MPEG_VIDC_VIDEO_ALLOC_MODE_INPUT	\
		(V4L2_CID_MPEG_MSM_VIDC_BASE+30)
#define V4L2_CID_MPEG_VIDC_VIDEO_ALLOC_MODE_OUTPUT	\
		 (V4L2_CID_MPEG_MSM_VIDC_BASE+31)
enum v4l2_mpeg_vidc_video_alloc_mode_type {
	V4L2_MPEG_VIDC_VIDEO_STATIC	= 0,
	V4L2_MPEG_VIDC_VIDEO_RING	= 1,
	V4L2_MPEG_VIDC_VIDEO_DYNAMIC	= 2,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_FRAME_ASSEMBLY	\
		(V4L2_CID_MPEG_MSM_VIDC_BASE+32)
enum v4l2_mpeg_vidc_video_assembly {
	V4L2_MPEG_VIDC_FRAME_ASSEMBLY_DISABLE	= 0,
	V4L2_MPEG_VIDC_FRAME_ASSEMBLY_ENABLE	= 1,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_VP8_PROFILE_LEVEL \
		(V4L2_CID_MPEG_MSM_VIDC_BASE+33)
enum v4l2_mpeg_vidc_video_vp8_profile_level {
	V4L2_MPEG_VIDC_VIDEO_VP8_UNUSED,
	V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_0,
	V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_1,
	V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_2,
	V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_3,
};

#define V4L2_CID_MPEG_VIDC_VIDEO_DEINTERLACE \
	(V4L2_CID_MPEG_MSM_VIDC_BASE + 34)
enum v4l2_mpeg_vidc_video_deinterlace {
	V4L2_CID_MPEG_VIDC_VIDEO_DEINTERLACE_DISABLED = 0,
	V4L2_CID_MPEG_VIDC_VIDEO_DEINTERLACE_ENABLED = 1
};
#define V4L2_CID_MPEG_VIDC_VIDEO_STREAM_OUTPUT_MODE \
		(V4L2_CID_MPEG_MSM_VIDC_BASE + 35)
enum v4l2_mpeg_vidc_video_decoder_multi_stream {
	V4L2_CID_MPEG_VIDC_VIDEO_STREAM_OUTPUT_PRIMARY = 0,
	V4L2_CID_MPEG_VIDC_VIDEO_STREAM_OUTPUT_SECONDARY = 1,
};

/*  Camera class control IDs */
#define V4L2_CID_CAMERA_CLASS_BASE 	(V4L2_CTRL_CLASS_CAMERA | 0x900)
#define V4L2_CID_CAMERA_CLASS 		(V4L2_CTRL_CLASS_CAMERA | 1)

#define V4L2_CID_EXPOSURE_AUTO			(V4L2_CID_CAMERA_CLASS_BASE+1)
enum  v4l2_exposure_auto_type {
	V4L2_EXPOSURE_AUTO = 0,
	V4L2_EXPOSURE_MANUAL = 1,
	V4L2_EXPOSURE_SHUTTER_PRIORITY = 2,
	V4L2_EXPOSURE_APERTURE_PRIORITY = 3
};
#define V4L2_CID_EXPOSURE_ABSOLUTE		(V4L2_CID_CAMERA_CLASS_BASE+2)
#define V4L2_CID_EXPOSURE_AUTO_PRIORITY		(V4L2_CID_CAMERA_CLASS_BASE+3)

#define V4L2_CID_PAN_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+4)
#define V4L2_CID_TILT_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+5)
#define V4L2_CID_PAN_RESET			(V4L2_CID_CAMERA_CLASS_BASE+6)
#define V4L2_CID_TILT_RESET			(V4L2_CID_CAMERA_CLASS_BASE+7)

#define V4L2_CID_PAN_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+8)
#define V4L2_CID_TILT_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+9)

#define V4L2_CID_FOCUS_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+10)
#define V4L2_CID_FOCUS_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+11)
#define V4L2_CID_FOCUS_AUTO			(V4L2_CID_CAMERA_CLASS_BASE+12)

#define V4L2_CID_ZOOM_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+13)
#define V4L2_CID_ZOOM_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+14)
#define V4L2_CID_ZOOM_CONTINUOUS		(V4L2_CID_CAMERA_CLASS_BASE+15)

#define V4L2_CID_PRIVACY			(V4L2_CID_CAMERA_CLASS_BASE+16)

#define V4L2_CID_IRIS_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+17)
#define V4L2_CID_IRIS_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+18)

/* FM Modulator class control IDs */
#define V4L2_CID_FM_TX_CLASS_BASE		(V4L2_CTRL_CLASS_FM_TX | 0x900)
#define V4L2_CID_FM_TX_CLASS			(V4L2_CTRL_CLASS_FM_TX | 1)

#define V4L2_CID_RDS_TX_DEVIATION		(V4L2_CID_FM_TX_CLASS_BASE + 1)
#define V4L2_CID_RDS_TX_PI			(V4L2_CID_FM_TX_CLASS_BASE + 2)
#define V4L2_CID_RDS_TX_PTY			(V4L2_CID_FM_TX_CLASS_BASE + 3)
#define V4L2_CID_RDS_TX_PS_NAME			(V4L2_CID_FM_TX_CLASS_BASE + 5)
#define V4L2_CID_RDS_TX_RADIO_TEXT		(V4L2_CID_FM_TX_CLASS_BASE + 6)

#define V4L2_CID_AUDIO_LIMITER_ENABLED		(V4L2_CID_FM_TX_CLASS_BASE + 64)
#define V4L2_CID_AUDIO_LIMITER_RELEASE_TIME	(V4L2_CID_FM_TX_CLASS_BASE + 65)
#define V4L2_CID_AUDIO_LIMITER_DEVIATION	(V4L2_CID_FM_TX_CLASS_BASE + 66)

#define V4L2_CID_AUDIO_COMPRESSION_ENABLED	(V4L2_CID_FM_TX_CLASS_BASE + 80)
#define V4L2_CID_AUDIO_COMPRESSION_GAIN		(V4L2_CID_FM_TX_CLASS_BASE + 81)
#define V4L2_CID_AUDIO_COMPRESSION_THRESHOLD	(V4L2_CID_FM_TX_CLASS_BASE + 82)
#define V4L2_CID_AUDIO_COMPRESSION_ATTACK_TIME	(V4L2_CID_FM_TX_CLASS_BASE + 83)
#define V4L2_CID_AUDIO_COMPRESSION_RELEASE_TIME	(V4L2_CID_FM_TX_CLASS_BASE + 84)

#define V4L2_CID_PILOT_TONE_ENABLED		(V4L2_CID_FM_TX_CLASS_BASE + 96)
#define V4L2_CID_PILOT_TONE_DEVIATION		(V4L2_CID_FM_TX_CLASS_BASE + 97)
#define V4L2_CID_PILOT_TONE_FREQUENCY		(V4L2_CID_FM_TX_CLASS_BASE + 98)

#define V4L2_CID_TUNE_PREEMPHASIS		(V4L2_CID_FM_TX_CLASS_BASE + 112)
enum v4l2_preemphasis {
	V4L2_PREEMPHASIS_DISABLED	= 0,
	V4L2_PREEMPHASIS_50_uS		= 1,
	V4L2_PREEMPHASIS_75_uS		= 2,
};
#define V4L2_CID_TUNE_POWER_LEVEL		(V4L2_CID_FM_TX_CLASS_BASE + 113)
#define V4L2_CID_TUNE_ANTENNA_CAPACITOR		(V4L2_CID_FM_TX_CLASS_BASE + 114)

/* Flash and privacy (indicator) light controls */
#define V4L2_CID_FLASH_CLASS_BASE		(V4L2_CTRL_CLASS_FLASH | 0x900)
#define V4L2_CID_FLASH_CLASS			(V4L2_CTRL_CLASS_FLASH | 1)

#define V4L2_CID_FLASH_LED_MODE			(V4L2_CID_FLASH_CLASS_BASE + 1)
enum v4l2_flash_led_mode {
	V4L2_FLASH_LED_MODE_NONE,
	V4L2_FLASH_LED_MODE_FLASH,
	V4L2_FLASH_LED_MODE_TORCH,
};

#define V4L2_CID_FLASH_STROBE_SOURCE		(V4L2_CID_FLASH_CLASS_BASE + 2)
enum v4l2_flash_strobe_source {
	V4L2_FLASH_STROBE_SOURCE_SOFTWARE,
	V4L2_FLASH_STROBE_SOURCE_EXTERNAL,
};

#define V4L2_CID_FLASH_STROBE			(V4L2_CID_FLASH_CLASS_BASE + 3)
#define V4L2_CID_FLASH_STROBE_STOP		(V4L2_CID_FLASH_CLASS_BASE + 4)
#define V4L2_CID_FLASH_STROBE_STATUS		(V4L2_CID_FLASH_CLASS_BASE + 5)

#define V4L2_CID_FLASH_TIMEOUT			(V4L2_CID_FLASH_CLASS_BASE + 6)
#define V4L2_CID_FLASH_INTENSITY		(V4L2_CID_FLASH_CLASS_BASE + 7)
#define V4L2_CID_FLASH_TORCH_INTENSITY		(V4L2_CID_FLASH_CLASS_BASE + 8)
#define V4L2_CID_FLASH_INDICATOR_INTENSITY	(V4L2_CID_FLASH_CLASS_BASE + 9)

#define V4L2_CID_FLASH_FAULT			(V4L2_CID_FLASH_CLASS_BASE + 10)
#define V4L2_FLASH_FAULT_OVER_VOLTAGE		(1 << 0)
#define V4L2_FLASH_FAULT_TIMEOUT		(1 << 1)
#define V4L2_FLASH_FAULT_OVER_TEMPERATURE	(1 << 2)
#define V4L2_FLASH_FAULT_SHORT_CIRCUIT		(1 << 3)
#define V4L2_FLASH_FAULT_OVER_CURRENT		(1 << 4)
#define V4L2_FLASH_FAULT_INDICATOR		(1 << 5)

#define V4L2_CID_FLASH_CHARGE			(V4L2_CID_FLASH_CLASS_BASE + 11)
#define V4L2_CID_FLASH_READY			(V4L2_CID_FLASH_CLASS_BASE + 12)

/*  JPEG-class control IDs defined by V4L2 */
#define V4L2_CID_JPEG_CLASS_BASE		(V4L2_CTRL_CLASS_JPEG | 0x900)
#define V4L2_CID_JPEG_CLASS			(V4L2_CTRL_CLASS_JPEG | 1)

#define	V4L2_CID_JPEG_CHROMA_SUBSAMPLING	(V4L2_CID_JPEG_CLASS_BASE + 1)
enum v4l2_jpeg_chroma_subsampling {
	V4L2_JPEG_CHROMA_SUBSAMPLING_444	= 0,
	V4L2_JPEG_CHROMA_SUBSAMPLING_422	= 1,
	V4L2_JPEG_CHROMA_SUBSAMPLING_420	= 2,
	V4L2_JPEG_CHROMA_SUBSAMPLING_411	= 3,
	V4L2_JPEG_CHROMA_SUBSAMPLING_410	= 4,
	V4L2_JPEG_CHROMA_SUBSAMPLING_GRAY	= 5,
};
#define	V4L2_CID_JPEG_RESTART_INTERVAL		(V4L2_CID_JPEG_CLASS_BASE + 2)
#define	V4L2_CID_JPEG_COMPRESSION_QUALITY	(V4L2_CID_JPEG_CLASS_BASE + 3)

#define	V4L2_CID_JPEG_ACTIVE_MARKER		(V4L2_CID_JPEG_CLASS_BASE + 4)
#define	V4L2_JPEG_ACTIVE_MARKER_APP0		(1 << 0)
#define	V4L2_JPEG_ACTIVE_MARKER_APP1		(1 << 1)
#define	V4L2_JPEG_ACTIVE_MARKER_COM		(1 << 16)
#define	V4L2_JPEG_ACTIVE_MARKER_DQT		(1 << 17)
#define	V4L2_JPEG_ACTIVE_MARKER_DHT		(1 << 18)

/*
 *	T U N I N G
 */
struct v4l2_tuner {
	__u32                   index;
	__u8			name[32];
	enum v4l2_tuner_type    type;
	__u32			capability;
	__u32			rangelow;
	__u32			rangehigh;
	__u32			rxsubchans;
	__u32			audmode;
	__s32			signal;
	__s32			afc;
	__u32			reserved[4];
};

struct v4l2_modulator {
	__u32			index;
	__u8			name[32];
	__u32			capability;
	__u32			rangelow;
	__u32			rangehigh;
	__u32			txsubchans;
	__u32			reserved[4];
};

/*  Flags for the 'capability' field */
#define V4L2_TUNER_CAP_LOW		0x0001
#define V4L2_TUNER_CAP_NORM		0x0002
#define V4L2_TUNER_CAP_STEREO		0x0010
#define V4L2_TUNER_CAP_LANG2		0x0020
#define V4L2_TUNER_CAP_SAP		0x0020
#define V4L2_TUNER_CAP_LANG1		0x0040
#define V4L2_TUNER_CAP_RDS		0x0080
#define V4L2_TUNER_CAP_RDS_BLOCK_IO	0x0100
#define V4L2_TUNER_CAP_RDS_CONTROLS	0x0200

/*  Flags for the 'rxsubchans' field */
#define V4L2_TUNER_SUB_MONO		0x0001
#define V4L2_TUNER_SUB_STEREO		0x0002
#define V4L2_TUNER_SUB_LANG2		0x0004
#define V4L2_TUNER_SUB_SAP		0x0004
#define V4L2_TUNER_SUB_LANG1		0x0008
#define V4L2_TUNER_SUB_RDS		0x0010

/*  Values for the 'audmode' field */
#define V4L2_TUNER_MODE_MONO		0x0000
#define V4L2_TUNER_MODE_STEREO		0x0001
#define V4L2_TUNER_MODE_LANG2		0x0002
#define V4L2_TUNER_MODE_SAP		0x0002
#define V4L2_TUNER_MODE_LANG1		0x0003
#define V4L2_TUNER_MODE_LANG1_LANG2	0x0004

struct v4l2_frequency {
	__u32		      tuner;
	enum v4l2_tuner_type  type;
	__u32		      frequency;
	__u32		      reserved[8];
};

struct v4l2_hw_freq_seek {
	__u32		      tuner;
	enum v4l2_tuner_type  type;
	__u32		      seek_upward;
	__u32		      wrap_around;
	__u32		      spacing;
	__u32		      reserved[7];
};

/*
 *	R D S
 */

struct v4l2_rds_data {
	__u8 	lsb;
	__u8 	msb;
	__u8 	block;
} __attribute__ ((packed));

#define V4L2_RDS_BLOCK_MSK 	 0x7
#define V4L2_RDS_BLOCK_A 	 0
#define V4L2_RDS_BLOCK_B 	 1
#define V4L2_RDS_BLOCK_C 	 2
#define V4L2_RDS_BLOCK_D 	 3
#define V4L2_RDS_BLOCK_C_ALT 	 4
#define V4L2_RDS_BLOCK_INVALID 	 7

#define V4L2_RDS_BLOCK_CORRECTED 0x40
#define V4L2_RDS_BLOCK_ERROR 	 0x80

/*
 *	A U D I O
 */
struct v4l2_audio {
	__u32	index;
	__u8	name[32];
	__u32	capability;
	__u32	mode;
	__u32	reserved[2];
};

/*  Flags for the 'capability' field */
#define V4L2_AUDCAP_STEREO		0x00001
#define V4L2_AUDCAP_AVL			0x00002

/*  Flags for the 'mode' field */
#define V4L2_AUDMODE_AVL		0x00001

struct v4l2_audioout {
	__u32	index;
	__u8	name[32];
	__u32	capability;
	__u32	mode;
	__u32	reserved[2];
};

/*
 *	M P E G   S E R V I C E S
 *
 *	NOTE: EXPERIMENTAL API
 */
#if 1
#define V4L2_ENC_IDX_FRAME_I    (0)
#define V4L2_ENC_IDX_FRAME_P    (1)
#define V4L2_ENC_IDX_FRAME_B    (2)
#define V4L2_ENC_IDX_FRAME_MASK (0xf)

struct v4l2_enc_idx_entry {
	__u64 offset;
	__u64 pts;
	__u32 length;
	__u32 flags;
	__u32 reserved[2];
};

#define V4L2_ENC_IDX_ENTRIES (64)
struct v4l2_enc_idx {
	__u32 entries;
	__u32 entries_cap;
	__u32 reserved[4];
	struct v4l2_enc_idx_entry entry[V4L2_ENC_IDX_ENTRIES];
};


#define V4L2_ENC_CMD_START      (0)
#define V4L2_ENC_CMD_STOP       (1)
#define V4L2_ENC_CMD_PAUSE      (2)
#define V4L2_ENC_CMD_RESUME     (3)
#define V4L2_ENC_QCOM_CMD_FLUSH  (4)

/* Flags for V4L2_ENC_CMD_STOP */
#define V4L2_ENC_CMD_STOP_AT_GOP_END    (1 << 0)

struct v4l2_encoder_cmd {
	__u32 cmd;
	__u32 flags;
	union {
		struct {
			__u32 data[8];
		} raw;
	};
};

/* Decoder commands */
#define V4L2_DEC_CMD_START       (0)
#define V4L2_DEC_CMD_STOP        (1)
#define V4L2_DEC_CMD_PAUSE       (2)
#define V4L2_DEC_CMD_RESUME      (3)
#define V4L2_DEC_QCOM_CMD_FLUSH  (4)

/* Flags for V4L2_DEC_CMD_START */
#define V4L2_DEC_CMD_START_MUTE_AUDIO	(1 << 0)

/* Flags for V4L2_DEC_CMD_PAUSE */
#define V4L2_DEC_CMD_PAUSE_TO_BLACK	(1 << 0)

/* Flags for V4L2_DEC_CMD_STOP */
#define V4L2_DEC_CMD_STOP_TO_BLACK	(1 << 0)
#define V4L2_DEC_CMD_STOP_IMMEDIATELY	(1 << 1)

/* Flags for V4L2_DEC_QCOM_CMD_FLUSH */
#define V4L2_DEC_QCOM_CMD_FLUSH_OUTPUT  (1 << 0)
#define V4L2_DEC_QCOM_CMD_FLUSH_CAPTURE (1 << 1)

#define V4L2_QCOM_CMD_FLUSH_OUTPUT  (1 << 0)
#define V4L2_QCOM_CMD_FLUSH_CAPTURE (1 << 1)

/* Play format requirements (returned by the driver): */

/* The decoder has no special format requirements */
#define V4L2_DEC_START_FMT_NONE		(0)
/* The decoder requires full GOPs */
#define V4L2_DEC_START_FMT_GOP		(1)

/* The structure must be zeroed before use by the application
   This ensures it can be extended safely in the future. */
struct v4l2_decoder_cmd {
	__u32 cmd;
	__u32 flags;
	union {
		struct {
			__u64 pts;
		} stop;

		struct {
			/* 0 or 1000 specifies normal speed,
			   1 specifies forward single stepping,
			   -1 specifies backward single stepping,
			   >1: playback at speed/1000 of the normal speed,
			   <-1: reverse playback at (-speed/1000) of the normal speed. */
			__s32 speed;
			__u32 format;
		} start;

		struct {
			__u32 data[16];
		} raw;
	};
};
#endif


/*
 *	D A T A   S E R V I C E S   ( V B I )
 *
 *	Data services API by Michael Schimek
 */

/* Raw VBI */
struct v4l2_vbi_format {
	__u32	sampling_rate;		/* in 1 Hz */
	__u32	offset;
	__u32	samples_per_line;
	__u32	sample_format;		/* V4L2_PIX_FMT_* */
	__s32	start[2];
	__u32	count[2];
	__u32	flags;			/* V4L2_VBI_* */
	__u32	reserved[2];		/* must be zero */
};

/*  VBI flags  */
#define V4L2_VBI_UNSYNC		(1 << 0)
#define V4L2_VBI_INTERLACED	(1 << 1)

/* Sliced VBI
 *
 *    This implements is a proposal V4L2 API to allow SLICED VBI
 * required for some hardware encoders. It should change without
 * notice in the definitive implementation.
 */

struct v4l2_sliced_vbi_format {
	__u16   service_set;
	/* service_lines[0][...] specifies lines 0-23 (1-23 used) of the first field
	   service_lines[1][...] specifies lines 0-23 (1-23 used) of the second field
				 (equals frame lines 313-336 for 625 line video
				  standards, 263-286 for 525 line standards) */
	__u16   service_lines[2][24];
	__u32   io_size;
	__u32   reserved[2];            /* must be zero */
};

/* Teletext World System Teletext
   (WST), defined on ITU-R BT.653-2 */
#define V4L2_SLICED_TELETEXT_B          (0x0001)
/* Video Program System, defined on ETS 300 231*/
#define V4L2_SLICED_VPS                 (0x0400)
/* Closed Caption, defined on EIA-608 */
#define V4L2_SLICED_CAPTION_525         (0x1000)
/* Wide Screen System, defined on ITU-R BT1119.1 */
#define V4L2_SLICED_WSS_625             (0x4000)

#define V4L2_SLICED_VBI_525             (V4L2_SLICED_CAPTION_525)
#define V4L2_SLICED_VBI_625             (V4L2_SLICED_TELETEXT_B | V4L2_SLICED_VPS | V4L2_SLICED_WSS_625)

struct v4l2_sliced_vbi_cap {
	__u16   service_set;
	/* service_lines[0][...] specifies lines 0-23 (1-23 used) of the first field
	   service_lines[1][...] specifies lines 0-23 (1-23 used) of the second field
				 (equals frame lines 313-336 for 625 line video
				  standards, 263-286 for 525 line standards) */
	__u16   service_lines[2][24];
	enum v4l2_buf_type type;
	__u32   reserved[3];    /* must be 0 */
};

struct v4l2_sliced_vbi_data {
	__u32   id;
	__u32   field;          /* 0: first field, 1: second field */
	__u32   line;           /* 1-23 */
	__u32   reserved;       /* must be 0 */
	__u8    data[48];
};

/*
 * Sliced VBI data inserted into MPEG Streams
 */

/*
 * V4L2_MPEG_STREAM_VBI_FMT_IVTV:
 *
 * Structure of payload contained in an MPEG 2 Private Stream 1 PES Packet in an
 * MPEG-2 Program Pack that contains V4L2_MPEG_STREAM_VBI_FMT_IVTV Sliced VBI
 * data
 *
 * Note, the MPEG-2 Program Pack and Private Stream 1 PES packet header
 * definitions are not included here.  See the MPEG-2 specifications for details
 * on these headers.
 */

/* Line type IDs */
#define V4L2_MPEG_VBI_IVTV_TELETEXT_B     (1)
#define V4L2_MPEG_VBI_IVTV_CAPTION_525    (4)
#define V4L2_MPEG_VBI_IVTV_WSS_625        (5)
#define V4L2_MPEG_VBI_IVTV_VPS            (7)

struct v4l2_mpeg_vbi_itv0_line {
	__u8 id;	/* One of V4L2_MPEG_VBI_IVTV_* above */
	__u8 data[42];	/* Sliced VBI data for the line */
} __attribute__ ((packed));

struct v4l2_mpeg_vbi_itv0 {
	__le32 linemask[2]; /* Bitmasks of VBI service lines present */
	struct v4l2_mpeg_vbi_itv0_line line[35];
} __attribute__ ((packed));

struct v4l2_mpeg_vbi_ITV0 {
	struct v4l2_mpeg_vbi_itv0_line line[36];
} __attribute__ ((packed));

#define V4L2_MPEG_VBI_IVTV_MAGIC0	"itv0"
#define V4L2_MPEG_VBI_IVTV_MAGIC1	"ITV0"

struct v4l2_mpeg_vbi_fmt_ivtv {
	__u8 magic[4];
	union {
		struct v4l2_mpeg_vbi_itv0 itv0;
		struct v4l2_mpeg_vbi_ITV0 ITV0;
	};
} __attribute__ ((packed));

/*
 *	A G G R E G A T E   S T R U C T U R E S
 */

/**
 * struct v4l2_plane_pix_format - additional, per-plane format definition
 * @sizeimage:		maximum size in bytes required for data, for which
 *			this plane will be used
 * @bytesperline:	distance in bytes between the leftmost pixels in two
 *			adjacent lines
 */
struct v4l2_plane_pix_format {
	__u32		sizeimage;
	__u16		bytesperline;
	__u16		reserved[7];
} __attribute__ ((packed));

/**
 * struct v4l2_pix_format_mplane - multiplanar format definition
 * @width:		image width in pixels
 * @height:		image height in pixels
 * @pixelformat:	little endian four character code (fourcc)
 * @field:		field order (for interlaced video)
 * @colorspace:		supplemental to pixelformat
 * @plane_fmt:		per-plane information
 * @num_planes:		number of planes for this format
 */
struct v4l2_pix_format_mplane {
	__u32				width;
	__u32				height;
	__u32				pixelformat;
	enum v4l2_field			field;
	enum v4l2_colorspace		colorspace;

	struct v4l2_plane_pix_format	plane_fmt[VIDEO_MAX_PLANES];
	__u8				num_planes;
	__u8				reserved[11];
} __attribute__ ((packed));

/**
 * struct v4l2_format - stream data format
 * @type:	type of the data stream
 * @pix:	definition of an image format
 * @pix_mp:	definition of a multiplanar image format
 * @win:	definition of an overlaid image
 * @vbi:	raw VBI capture or output parameters
 * @sliced:	sliced VBI capture or output parameters
 * @raw_data:	placeholder for future extensions and custom formats
 */
struct v4l2_format {
  enum v4l2_buf_type type;
  struct v4l2_window		win;     /* V4L2_BUF_TYPE_VIDEO_OVERLAY */

};

/*	Stream type-dependent parameters
 */
struct v4l2_streamparm {
	enum v4l2_buf_type type;
	union {
		struct v4l2_captureparm	capture;
		struct v4l2_outputparm	output;
		__u8	raw_data[200];  /* user-defined */
	} parm;
};

/*
 *	E V E N T S
 */

#define V4L2_EVENT_ALL				0
#define V4L2_EVENT_VSYNC			1
#define V4L2_EVENT_EOS				2
#define V4L2_EVENT_CTRL				3
#define V4L2_EVENT_FRAME_SYNC			4
#define V4L2_EVENT_PRIVATE_START		0x08000000

#define V4L2_EVENT_MSM_VIDC_START	(V4L2_EVENT_PRIVATE_START + 0x00001000)
#define V4L2_EVENT_MSM_VIDC_FLUSH_DONE	(V4L2_EVENT_MSM_VIDC_START + 1)
#define V4L2_EVENT_MSM_VIDC_PORT_SETTINGS_CHANGED_SUFFICIENT	\
		(V4L2_EVENT_MSM_VIDC_START + 2)
#define V4L2_EVENT_MSM_VIDC_PORT_SETTINGS_CHANGED_INSUFFICIENT	\
		(V4L2_EVENT_MSM_VIDC_START + 3)
#define V4L2_EVENT_MSM_VIDC_CLOSE_DONE	(V4L2_EVENT_MSM_VIDC_START + 4)
#define V4L2_EVENT_MSM_VIDC_SYS_ERROR	(V4L2_EVENT_MSM_VIDC_START + 5)
#define V4L2_EVENT_MSM_VIDC_RELEASE_BUFFER_REFERENCE \
		(V4L2_EVENT_MSM_VIDC_START + 6)
#define V4L2_EVENT_MSM_VIDC_RELEASE_UNQUEUED_BUFFER \
		(V4L2_EVENT_MSM_VIDC_START + 7)

/* Payload for V4L2_EVENT_VSYNC */
struct v4l2_event_vsync {
	/* Can be V4L2_FIELD_ANY, _NONE, _TOP or _BOTTOM */
	__u8 field;
} __attribute__ ((packed));

/* Payload for V4L2_EVENT_CTRL */
#define V4L2_EVENT_CTRL_CH_VALUE		(1 << 0)
#define V4L2_EVENT_CTRL_CH_FLAGS		(1 << 1)

struct v4l2_event_ctrl {
	__u32 changes;
	__u32 type;
	union {
		__s32 value;
		__s64 value64;
	};
	__u32 flags;
	__s32 minimum;
	__s32 maximum;
	__s32 step;
	__s32 default_value;
};

struct v4l2_event_frame_sync {
	__u32 frame_sequence;
};

struct v4l2_event {
	__u32				type;
	union {
		struct v4l2_event_vsync		vsync;
		struct v4l2_event_ctrl		ctrl;
		struct v4l2_event_frame_sync	frame_sync;
		__u8				data[64];
	} u;
	__u32				pending;
	__u32				sequence;
	struct timespec			timestamp;
	__u32				id;
	__u32				reserved[8];
};

#define V4L2_EVENT_SUB_FL_SEND_INITIAL		(1 << 0)
#define V4L2_EVENT_SUB_FL_ALLOW_FEEDBACK	(1 << 1)

struct v4l2_event_subscription {
	__u32				type;
	__u32				id;
	__u32				flags;
	__u32				reserved[5];
};

/*
 *	A D V A N C E D   D E B U G G I N G
 *
 *	NOTE: EXPERIMENTAL API, NEVER RELY ON THIS IN APPLICATIONS!
 *	FOR DEBUGGING, TESTING AND INTERNAL USE ONLY!
 */

/* VIDIOC_DBG_G_REGISTER and VIDIOC_DBG_S_REGISTER */

#define V4L2_CHIP_MATCH_HOST       0  /* Match against chip ID on host (0 for the host) */
#define V4L2_CHIP_MATCH_I2C_DRIVER 1  /* Match against I2C driver name */
#define V4L2_CHIP_MATCH_I2C_ADDR   2  /* Match against I2C 7-bit address */
#define V4L2_CHIP_MATCH_AC97       3  /* Match against anciliary AC97 chip */

struct v4l2_dbg_match {
	__u32 type; /* Match type */
	union {     /* Match this chip, meaning determined by type */
		__u32 addr;
		char name[32];
	};
} __attribute__ ((packed));

struct v4l2_dbg_register {
	struct v4l2_dbg_match match;
	__u32 size;	/* register size in bytes */
	__u64 reg;
	__u64 val;
} __attribute__ ((packed));

/* VIDIOC_DBG_G_CHIP_IDENT */
struct v4l2_dbg_chip_ident {
	struct v4l2_dbg_match match;
	__u32 ident;       /* chip identifier as specified in <media/v4l2-chip-ident.h> */
	__u32 revision;    /* chip revision, chip specific */
} __attribute__ ((packed));

/**
 * struct v4l2_create_buffers - VIDIOC_CREATE_BUFS argument
 * @index:	on return, index of the first created buffer
 * @count:	entry: number of requested buffers,
 *		return: number of created buffers
 * @memory:	buffer memory type
 * @format:	frame format, for which buffers are requested
 * @reserved:	future extensions
 */
struct v4l2_create_buffers {
	__u32			index;
	__u32			count;
	enum v4l2_memory        memory;
	struct v4l2_format	format;
	__u32			reserved[8];
};

/*
 *	I O C T L   C O D E S   F O R   V I D E O   D E V I C E S
 *
 */
#define VIDIOC_QUERYCAP		 _IOR('V',  0, struct v4l2_capability)
#define VIDIOC_RESERVED		  _IO('V',  1)
#define VIDIOC_ENUM_FMT         _IOWR('V',  2, struct v4l2_fmtdesc)
#define VIDIOC_G_FMT		_IOWR('V',  4, struct v4l2_format)
#define VIDIOC_S_FMT		_IOWR('V',  5, struct v4l2_format)
#define VIDIOC_REQBUFS		_IOWR('V',  8, struct v4l2_requestbuffers)
#define VIDIOC_QUERYBUF		_IOWR('V',  9, struct v4l2_buffer)
#define VIDIOC_G_FBUF		 _IOR('V', 10, struct v4l2_framebuffer)
#define VIDIOC_S_FBUF		 _IOW('V', 11, struct v4l2_framebuffer)
#define VIDIOC_OVERLAY		 _IOW('V', 14, int)
#define VIDIOC_QBUF		_IOWR('V', 15, struct v4l2_buffer)
#define VIDIOC_DQBUF		_IOWR('V', 17, struct v4l2_buffer)
#define VIDIOC_STREAMON		 _IOW('V', 18, int)
#define VIDIOC_STREAMOFF	 _IOW('V', 19, int)
#define VIDIOC_G_PARM		_IOWR('V', 21, struct v4l2_streamparm)
#define VIDIOC_S_PARM		_IOWR('V', 22, struct v4l2_streamparm)
#define VIDIOC_G_STD		 _IOR('V', 23, v4l2_std_id)
#define VIDIOC_S_STD		 _IOW('V', 24, v4l2_std_id)
#define VIDIOC_ENUMSTD		_IOWR('V', 25, struct v4l2_standard)
#define VIDIOC_ENUMINPUT	_IOWR('V', 26, struct v4l2_input)
#define VIDIOC_G_CTRL		_IOWR('V', 27, struct v4l2_control)
#define VIDIOC_S_CTRL		_IOWR('V', 28, struct v4l2_control)
#define VIDIOC_G_TUNER		_IOWR('V', 29, struct v4l2_tuner)
#define VIDIOC_S_TUNER		 _IOW('V', 30, struct v4l2_tuner)
#define VIDIOC_G_AUDIO		 _IOR('V', 33, struct v4l2_audio)
#define VIDIOC_S_AUDIO		 _IOW('V', 34, struct v4l2_audio)
#define VIDIOC_QUERYCTRL	_IOWR('V', 36, struct v4l2_queryctrl)
#define VIDIOC_QUERYMENU	_IOWR('V', 37, struct v4l2_querymenu)
#define VIDIOC_G_INPUT		 _IOR('V', 38, int)
#define VIDIOC_S_INPUT		_IOWR('V', 39, int)
#define VIDIOC_G_OUTPUT		 _IOR('V', 46, int)
#define VIDIOC_S_OUTPUT		_IOWR('V', 47, int)
#define VIDIOC_ENUMOUTPUT	_IOWR('V', 48, struct v4l2_output)
#define VIDIOC_G_AUDOUT		 _IOR('V', 49, struct v4l2_audioout)
#define VIDIOC_S_AUDOUT		 _IOW('V', 50, struct v4l2_audioout)
#define VIDIOC_G_MODULATOR	_IOWR('V', 54, struct v4l2_modulator)
#define VIDIOC_S_MODULATOR	 _IOW('V', 55, struct v4l2_modulator)
#define VIDIOC_G_FREQUENCY	_IOWR('V', 56, struct v4l2_frequency)
#define VIDIOC_S_FREQUENCY	 _IOW('V', 57, struct v4l2_frequency)
#define VIDIOC_CROPCAP		_IOWR('V', 58, struct v4l2_cropcap)
#define VIDIOC_G_CROP		_IOWR('V', 59, struct v4l2_crop)
#define VIDIOC_S_CROP		 _IOW('V', 60, struct v4l2_crop)
#define VIDIOC_G_JPEGCOMP	 _IOR('V', 61, struct v4l2_jpegcompression)
#define VIDIOC_S_JPEGCOMP	 _IOW('V', 62, struct v4l2_jpegcompression)
#define VIDIOC_QUERYSTD      	 _IOR('V', 63, v4l2_std_id)
#define VIDIOC_TRY_FMT      	_IOWR('V', 64, struct v4l2_format)
#define VIDIOC_ENUMAUDIO	_IOWR('V', 65, struct v4l2_audio)
#define VIDIOC_ENUMAUDOUT	_IOWR('V', 66, struct v4l2_audioout)
#define VIDIOC_G_PRIORITY        _IOR('V', 67, enum v4l2_priority)
#define VIDIOC_S_PRIORITY        _IOW('V', 68, enum v4l2_priority)
#define VIDIOC_G_SLICED_VBI_CAP _IOWR('V', 69, struct v4l2_sliced_vbi_cap)
#define VIDIOC_LOG_STATUS         _IO('V', 70)
#define VIDIOC_G_EXT_CTRLS	_IOWR('V', 71, struct v4l2_ext_controls)
#define VIDIOC_S_EXT_CTRLS	_IOWR('V', 72, struct v4l2_ext_controls)
#define VIDIOC_TRY_EXT_CTRLS	_IOWR('V', 73, struct v4l2_ext_controls)
//#if 1
#define VIDIOC_ENUM_FRAMESIZES	_IOWR('V', 74, struct v4l2_frmsizeenum)
#define VIDIOC_ENUM_FRAMEINTERVALS _IOWR('V', 75, struct v4l2_frmivalenum)
#define VIDIOC_G_ENC_INDEX       _IOR('V', 76, struct v4l2_enc_idx)
#define VIDIOC_ENCODER_CMD      _IOWR('V', 77, struct v4l2_encoder_cmd)
#define VIDIOC_TRY_ENCODER_CMD  _IOWR('V', 78, struct v4l2_encoder_cmd)
//#endif

#if 1
/* Experimental, meant for debugging, testing and internal use.
   Only implemented if CONFIG_VIDEO_ADV_DEBUG is defined.
   You must be root to use these ioctls. Never use these in applications! */
#define	VIDIOC_DBG_S_REGISTER 	 _IOW('V', 79, struct v4l2_dbg_register)
#define	VIDIOC_DBG_G_REGISTER 	_IOWR('V', 80, struct v4l2_dbg_register)

/* Experimental, meant for debugging, testing and internal use.
   Never use this ioctl in applications! */
#define VIDIOC_DBG_G_CHIP_IDENT _IOWR('V', 81, struct v4l2_dbg_chip_ident)
#endif

#define VIDIOC_S_HW_FREQ_SEEK	 _IOW('V', 82, struct v4l2_hw_freq_seek)
#define	VIDIOC_ENUM_DV_PRESETS	_IOWR('V', 83, struct v4l2_dv_enum_preset)
#define	VIDIOC_S_DV_PRESET	_IOWR('V', 84, struct v4l2_dv_preset)
#define	VIDIOC_G_DV_PRESET	_IOWR('V', 85, struct v4l2_dv_preset)
#define	VIDIOC_QUERY_DV_PRESET	_IOR('V',  86, struct v4l2_dv_preset)
#define	VIDIOC_S_DV_TIMINGS	_IOWR('V', 87, struct v4l2_dv_timings)
#define	VIDIOC_G_DV_TIMINGS	_IOWR('V', 88, struct v4l2_dv_timings)
#define	VIDIOC_DQEVENT		 _IOR('V', 89, struct v4l2_event)
#define	VIDIOC_SUBSCRIBE_EVENT	 _IOW('V', 90, struct v4l2_event_subscription)
#define	VIDIOC_UNSUBSCRIBE_EVENT _IOW('V', 91, struct v4l2_event_subscription)

/* Experimental, the below two ioctls may change over the next couple of kernel
   versions */
#define VIDIOC_CREATE_BUFS	_IOWR('V', 92, struct v4l2_create_buffers)
#define VIDIOC_PREPARE_BUF	_IOWR('V', 93, struct v4l2_buffer)

/* Experimental selection API */
#define VIDIOC_G_SELECTION	_IOWR('V', 94, struct v4l2_selection)
#define VIDIOC_S_SELECTION	_IOWR('V', 95, struct v4l2_selection)

/* Experimental, these two ioctls may change over the next couple of kernel
   versions. */
#define VIDIOC_DECODER_CMD	_IOWR('V', 96, struct v4l2_decoder_cmd)
#define VIDIOC_TRY_DECODER_CMD	_IOWR('V', 97, struct v4l2_decoder_cmd)

/* Reminder: when adding new ioctls please add support for them to
   drivers/media/video/v4l2-compat-ioctl32.c as well! */

#define BASE_VIDIOC_PRIVATE	192		/* 192-255 are private */





/***********************************************************/
/********************* MOBICORE ****************************/
/***********************************************************/




#define MC_ADMIN_DEVNODE	"mobicore"
#define MC_USER_DEVNODE		"mobicore-user"

/*
 * Data exchange structure of the MC_DRV_MODULE_INIT ioctl command.
 * INIT request data to SWD
 */
struct mc_ioctl_init {
	/* notification buffer start/length [16:16] [start, length] */
	uint32_t  nq_offset;
	/* length of notification queue */
	uint32_t  nq_length;
	/* mcp buffer start/length [16:16] [start, length] */
	uint32_t  mcp_offset;
	/* length of mcp buffer */
	uint32_t  mcp_length;
};

/*
 * Data exchange structure of the MC_DRV_MODULE_INFO ioctl command.
 * INFO request data to the SWD
 */
struct mc_ioctl_info {
	uint32_t  ext_info_id;	/* extended info ID */
	uint32_t  state;	/* state */
	uint32_t  ext_info;	/* extended info */
};

/*
 * Data exchange structure of the MC_IO_MAP_WSM, MC_IO_MAP_MCI, and
 *				  MC_IO_MAP_PWSM commands.
 *
 * Allocate a contiguous memory buffer for a process.
 * The physical address can be used as for later calls to mmap.
 * The handle can be used to communicate about this buffer to the Daemon.
 * For MC_IO_MAP_MCI command, the reused field indicates that MCI was set up
 * already. I.e. Daemon was restarted.
 */
struct mc_ioctl_map {
	size_t	      len;	/* Buffer length */
	uint32_t      handle;	/* WSM handle */
	unsigned long addr;	/* Virtual address */
	unsigned long phys_addr;/* physical address of WSM (or NULL) */
	bool	      reused;	/* if WSM memory was reused, or new allocated */
};

/*
 * Data exchange structure of the MC_IO_REG_WSM command.
 *
 * Allocates a physical L2 table and maps the buffer into this page.
 * Returns the physical address of the L2 table.
 * The page alignment will be created and the appropriated pSize and pOffsetL2
 * will be modified to the used values.
 */
struct mc_ioctl_reg_wsm {
	uint32_t buffer;	/* base address of the virtual address  */
	uint32_t len;		/* size of the virtual address space */
	uint32_t pid;		/* process id */
	uint32_t handle;	/* driver handle for locked memory */
	uint32_t table_phys;	/* physical address of the L2 table */
};


/*
 * Data exchange structure of the MC_DRV_MODULE_FC_EXECUTE ioctl command.
 * internal, unsupported
 */
struct mc_ioctl_execute {
	/* base address of mobicore binary */
	uint32_t phys_start_addr;
	/* length of DDR area */
	uint32_t length;
};

/*
 * Data exchange structure of the MC_IO_RESOLVE_CONT_WSM ioctl command.
 */
struct mc_ioctl_resolv_cont_wsm {
	/* driver handle for buffer */
	uint32_t handle;
	/* base address of memory */
	uint32_t phys;
	/* length memory */
	uint32_t length;
};


struct mc_ioctl_resolv_wsm {
  /* driver handle for buffer */
  uint32_t handle;
  /* fd to owner of the buffer */
  int32_t fd;
  /* base address of memory */
  uint32_t phys;
};


/*
 * defines for the ioctl mobicore driver module function call from user space.
 */
/* MobiCore IOCTL magic number */
#define MC_IOC_MAGIC	'M'

#define MC_IO_INIT		_IOWR(MC_IOC_MAGIC, 0, struct mc_ioctl_init)
#define MC_IO_INFO		_IOWR(MC_IOC_MAGIC, 1, struct mc_ioctl_info)
#define MC_IO_VERSION		_IOR(MC_IOC_MAGIC, 2, uint32_t)
/*
 * ioctl parameter to send the YIELD command to the SWD.
 * Only possible in Privileged Mode.
 * ioctl(fd, MC_DRV_MODULE_YIELD)
 */
#define MC_IO_YIELD		_IO(MC_IOC_MAGIC, 3)
/*
 * ioctl parameter to send the NSIQ signal to the SWD.
 * Only possible in Privileged Mode
 * ioctl(fd, MC_DRV_MODULE_NSIQ)
 */
#define MC_IO_NSIQ		_IO(MC_IOC_MAGIC, 4)
/*
 * Free's memory which is formerly allocated by the driver's mmap
 * command. The parameter must be this mmaped address.
 * The internal instance data regarding to this address are deleted as
 * well as each according memory page and its appropriated reserved bit
 * is cleared (ClearPageReserved).
 * Usage: ioctl(fd, MC_DRV_MODULE_FREE, &address) with address being of
 * type long address
 */
#define MC_IO_FREE		_IO(MC_IOC_MAGIC, 5)
/*
 * Creates a L2 Table of the given base address and the size of the
 * data.
 * Parameter: mc_ioctl_app_reg_wsm_l2_params
 */
#define MC_IO_REG_WSM		_IOWR(MC_IOC_MAGIC, 6, struct mc_ioctl_reg_wsm)
#define MC_IO_UNREG_WSM		_IO(MC_IOC_MAGIC, 7)
#define MC_IO_LOCK_WSM		_IO(MC_IOC_MAGIC, 8)
#define MC_IO_UNLOCK_WSM	_IO(MC_IOC_MAGIC, 9)
#define MC_IO_EXECUTE		_IOWR(MC_IOC_MAGIC, 10, struct mc_ioctl_execute)

/*
 * Allocate contiguous memory for a process for later mapping with mmap.
 * MC_DRV_KMOD_MMAP_WSM	usual operation, pages are registered in
 *					device structure and freed later.
 * MC_DRV_KMOD_MMAP_MCI	get Instance of MCI, allocates or mmaps
 *					the MCI to daemon
 * MC_DRV_KMOD_MMAP_PERSISTENTWSM	special operation, without
 *						registration of pages
 */
#define MC_IO_MAP_WSM		_IOWR(MC_IOC_MAGIC, 11, struct mc_ioctl_map)
#define MC_IO_MAP_MCI		_IOWR(MC_IOC_MAGIC, 12, struct mc_ioctl_map)
#define MC_IO_MAP_PWSM		_IOWR(MC_IOC_MAGIC, 13, struct mc_ioctl_map)




/*
 * Clean orphaned WSM buffers. Only available to the daemon and should
 * only be carried out if the TLC crashes or otherwise calls exit() in
 * an unexpected manner.
 * The clean is needed together with the lock/unlock mechanism so the daemon
 * has clear control of the mapped buffers so it can close a Trustlet before
 * release all the WSM buffers, otherwise the Trustlet would be able to write
 * to possibly kernel memory areas
 */
#define MC_IO_CLEAN_WSM		_IO(MC_IOC_MAGIC, 14)

/*
 * Get L2 phys address of a buffer handle allocated to the user.
 * Only available to the daemon.
 */
#define MC_IO_RESOLVE_WSM	_IOWR(MC_IOC_MAGIC, 15, uint32_t)

/*
 * Get the phys address & length of a allocated contiguous buffer.
 * Only available to the daemon */
#define MC_IO_RESOLVE_CONT_WSM	_IOWR(MC_IOC_MAGIC, 16, struct mc_ioctl_execute)


/*
 * Setup the mem traces when called.
 * Only available to the daemon */
#define MC_IO_LOG_SETUP _IO(MC_IOC_MAGIC, 17)

/*******************************************************************/
/***************MSM_JPEG *******************************************/


#define OUTPUT_H2V1  0
#define OUTPUT_H2V2  1
#define OUTPUT_BYTE  6

#define MSM_JPEG_IOCTL_MAGIC 'g'

#define MSM_JPEG_IOCTL_GET_HW_VERSION \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 1, struct msm_jpeg_hw_cmd *)

#define MSM_JPEG_IOCTL_RESET \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 2, struct msm_jpeg_ctrl_cmd *)

#define MSM_JPEG_IOCTL_STOP \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 3, struct msm_jpeg_hw_cmds *)

#define MSM_JPEG_IOCTL_START \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 4, struct msm_jpeg_hw_cmds *)

#define MSM_JPEG_IOCTL_INPUT_BUF_ENQUEUE \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 5, struct msm_jpeg_buf *)

#define MSM_JPEG_IOCTL_INPUT_GET \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 6, struct msm_jpeg_buf *)

#define MSM_JPEG_IOCTL_INPUT_GET_UNBLOCK \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 7, int)

#define MSM_JPEG_IOCTL_OUTPUT_BUF_ENQUEUE \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 8, struct msm_jpeg_buf *)

#define MSM_JPEG_IOCTL_OUTPUT_GET \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 9, struct msm_jpeg_buf *)

#define MSM_JPEG_IOCTL_OUTPUT_GET_UNBLOCK \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 10, int)

#define MSM_JPEG_IOCTL_EVT_GET \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 11, struct msm_jpeg_ctrl_cmd *)

#define MSM_JPEG_IOCTL_EVT_GET_UNBLOCK \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 12, int)

#define MSM_JPEG_IOCTL_HW_CMD \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 13, struct msm_jpeg_hw_cmd *)

#define MSM_JPEG_IOCTL_HW_CMDS \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 14, struct msm_jpeg_hw_cmds *)

#define MSM_JPEG_IOCTL_TEST_DUMP_REGION \
  _IOW(MSM_JPEG_IOCTL_MAGIC, 15, unsigned long)






#define MSM_JPEG_MODE_REALTIME_ENCODE 0
#define MSM_JPEG_MODE_OFFLINE_ENCODE 1
#define MSM_JPEG_MODE_REALTIME_ROTATION 2
#define MSM_JPEG_MODE_OFFLINE_ROTATION 3

  struct msm_jpeg_ctrl_cmd {
    uint32_t type;
    uint32_t len;
    void     *value;
  };

#define MSM_JPEG_EVT_RESET 0
#define MSM_JPEG_EVT_SESSION_DONE1
#define MSM_JPEG_EVT_ERR 2

struct msm_jpeg_buf {
uint32_t type;
int      fd;

void     *vaddr;

uint32_t y_off;
uint32_t y_len;
uint32_t framedone_len;

uint32_t cbcr_off;
uint32_t cbcr_len;

uint32_t num_of_mcu_rows;
uint32_t offset;
uint32_t pln2_off;
uint32_t pln2_len;
};

#define MSM_JPEG_HW_CMD_TYPE_READ      0
#define MSM_JPEG_HW_CMD_TYPE_WRITE     1
#define MSM_JPEG_HW_CMD_TYPE_WRITE_OR  2
#define MSM_JPEG_HW_CMD_TYPE_UWAIT     3
#define MSM_JPEG_HW_CMD_TYPE_MWAIT     4
#define MSM_JPEG_HW_CMD_TYPE_MDELAY    5
#define MSM_JPEG_HW_CMD_TYPE_UDELAY    6
struct msm_jpeg_hw_cmd {

uint32_t type:4;

/* n microseconds of timeout for WAIT */
/* n microseconds of time for DELAY */
/* repeat n times for READ/WRITE */
/* max is 0xFFF, 4095 */
uint32_t n:12;
uint32_t offset:16;
uint32_t mask;
union {
uint32_t data;   /* for single READ/WRITE/WAIT, n = 1 */
uint32_t *pdata;   /* for multiple READ/WRITE/WAIT, n > 1 */
};
};

struct msm_jpeg_hw_cmds {
uint32_t m; /* number of elements in the hw_cmd array */
struct msm_jpeg_hw_cmd hw_cmd[1];
};










