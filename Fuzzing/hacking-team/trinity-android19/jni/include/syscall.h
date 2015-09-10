#pragma once

#include <sys/time.h>
#include <sys/types.h>
#include "locks.h"
#include "types.h"

#define PREBUFFER_LEN	4096 * 6
#define POSTBUFFER_LEN	128

#define MAX_NR_SYSCALL 1024

enum ioctl_struct_type {
  STRUCT_undefined = 0,
  // MSM_VIDC_ENC
  STRUCT_venc_ioctl_msg,
  // MSM_VIDC_DEC
  STRUCT_vdec_ioctl_msg,

  STRUCT_qseecom_register_listener_req,
  STRUCT_qseecom_send_cmd_req,
  STRUCT_qseecom_ion_fd_info,
  STRUCT_qseecom_send_modfd_cmd_req,
  STRUCT_qseecom_send_resp_req,
  STRUCT_qseecom_load_img_req,
  STRUCT_qseecom_set_sb_mem_param_req,
  STRUCT_qseecom_qseos_version_req,
  STRUCT_qseecom_qseos_app_load_query,
  STRUCT_kgsl_devinfo,
  STRUCT_kgsl_devmemstore,
  STRUCT_kgsl_shadowprop,
  STRUCT_kgsl_version,
  STRUCT_kgsl_ibdesc,
  STRUCT_kgsl_device_getproperty,
  STRUCT_kgsl_device_waittimestamp,
  STRUCT_kgsl_device_waittimestamp_ctxtid,
  STRUCT_kgsl_ringbuffer_issueibcmds,
  STRUCT_kgsl_cmdstream_readtimestamp,
  STRUCT_kgsl_cmdstream_freememontimestamp,
  STRUCT_kgsl_drawctxt_create,
  STRUCT_kgsl_map_user_mem,
  STRUCT_kgsl_cmdstream_readtimestamp_ctxtid,
  STRUCT_kgsl_cmdstream_freememontimestamp_ctxtid,
  STRUCT_kgsl_sharedmem_from_pmem,
  STRUCT_kgsl_sharedmem_free,
  STRUCT_kgsl_cff_user_event,
  STRUCT_kgsl_gmem_desc,
  STRUCT_kgsl_buffer_desc,
  STRUCT_kgsl_bind_gmem_shadow,
  STRUCT_kgsl_sharedmem_from_vmalloc,
  STRUCT_kgsl_drawctxt_set_bin_base_offset,
  STRUCT_kgsl_cmdwindow_write,
  STRUCT_kgsl_cff_syncmem,
  STRUCT_kgsl_timestamp_event,
  STRUCT_kgsl_timestamp_event_genlock,
  STRUCT_kgsl_timestamp_event_fence,
  STRUCT_kgsl_gpumem_alloc_id,
  STRUCT_kgsl_gpumem_free_id,
  STRUCT_kgsl_gpumem_get_info,
  STRUCT_kgsl_gpumem_sync_cache,
  STRUCT_kgsl_perfcounter_get,
  STRUCT_kgsl_perfcounter_put,
  STRUCT_kgsl_perfcounter_query,
  STRUCT_kgsl_perfcounter_read_group,
  STRUCT_kgsl_perfcounter_read,
  STRUCT_kgsl_drawctxt_destroy,
  STRUCT_kgsl_gpumem_alloc,
  STRUCT_mc_ioctl_init,
  STRUCT_mc_ioctl_info,
  STRUCT_mc_ioctl_map,
  STRUCT_mc_ioctl_reg_wsm,
  STRUCT_mc_ioctl_execute,
  STRUCT_mc_ioctl_resolv_cont_wsm,
  STRUCT_mc_ioctl_resolv_wsm,
  STRUCT_msm_jpeg_ctrl_cmd,
  STRUCT_msm_jpeg_buf,
  STRUCT_msm_jpeg_hw_cmd,
  STRUCT_msm_jpeg_hw_cmds,
  STRUCT_timespec,
  STRUCT_v4l2_capability,
  STRUCT_v4l2_fmtdesc,
  STRUCT_v4l2_rect,
  STRUCT_v4l2_clip,
  STRUCT_v4l2_window,
  STRUCT_v4l2_format,
  STRUCT_v4l2_requestbuffers,
  STRUCT_v4l2_timecode,
  STRUCT_v4l2_plane,
  STRUCT_timeval,
  STRUCT_v4l2_buffer,
  STRUCT_v4l2_plane_pix_format,
  STRUCT_v4l2_pix_format_mplane,
  STRUCT_v4l2_pix_format,
  STRUCT_v4l2_framebuffer,
  STRUCT_v4l2_streamparm,
  STRUCT_v4l2_fract,
  STRUCT_v4l2_standard,
  STRUCT_v4l2_input,
  STRUCT_v4l2_control,
  STRUCT_v4l2_modulator,
  STRUCT_v4l2_audio,
  STRUCT_v4l2_queryctrl,
  STRUCT_v4l2_querymenu,
  STRUCT_v4l2_outputparm,
  STRUCT_v4l2_audioout,
  STRUCT_v4l2_frequency,
  STRUCT_v4l2_cropcap,
  STRUCT_v4l2_crop,
  STRUCT_v4l2_jpegcompression,
  STRUCT_v4l2_sliced_vbi_cap,
  STRUCT_v4l2_ext_controls,
  STRUCT_v4l2_frmsize_stepwise,
  STRUCT_v4l2_frmsizeenum,
  STRUCT_v4l2_frmival_stepwise,
  STRUCT_v4l2_frmivalenum,
  STRUCT_v4l2_enc_idx_entry,
  STRUCT_v4l2_enc_idx,
  STRUCT_v4l2_encoder_cmd,
  STRUCT_v4l2_dbg_match,
  STRUCT_v4l2_dbg_register,
  STRUCT_v4l2_dbg_chip_ident,
  STRUCT_v4l2_hw_freq_seek,
  STRUCT_v4l2_dv_enum_preset,
  STRUCT_v4l2_dv_preset,
  STRUCT_v4l2_dv_timings,
  STRUCT_v4l2_event,
  STRUCT_v4l2_event_subscription,
  STRUCT_v4l2_create_buffers,
  STRUCT_v4l2_selection,
  STRUCT_v4l2_decoder_cmd,
  STRUCT_v4l2_tuner,
  STRUCT_v4l2_output

};  

enum syscallstate {
	UNKNOWN,	/* new child */
	PREP,		/* doing sanitize */
	BEFORE,		/* about to do syscall */
	GOING_AWAY,	/* used when we don't expect to come back (execve for eg) */
	AFTER,		/* returned from doing syscall. */
	DONE,		/* moved to previous */
};

struct syscallrecord {
  struct timeval tv;
  unsigned int nr;
  unsigned long a1;
  unsigned long a2;
  unsigned long a3;
  unsigned long a4;
  unsigned long a5;
  unsigned long a6;
  unsigned long retval;
  int errno_post;	/* what errno was after the syscall. */
  
  unsigned long op_nr;	/* used to tell if we're making progress. */

  bool do32bit;
  lock_t lock;
  enum syscallstate state;
  char prebuffer[PREBUFFER_LEN];
  char postbuffer[POSTBUFFER_LEN];
  bool is_ioctl_call;
  enum ioctl_struct_type ioctl_struct_type;
};

enum argtype {
  ARG_UNDEFINED,
  ARG_FD,
  ARG_LEN,
  ARG_ADDRESS,
  ARG_MODE_T,
  ARG_NON_NULL_ADDRESS,
  ARG_PID,
  ARG_RANGE,
  ARG_OP,
  ARG_LIST,
  ARG_CPU,
  ARG_PATHNAME,
  ARG_IOVEC,
  ARG_IOVECLEN,
  ARG_SOCKADDR,
  ARG_SOCKADDRLEN,
  ARG_MMAP,
};

enum struct_argtype {

  STRUCT_ARG_timespec = 1,
  STRUCT_ARG_timeval,
  STRUCT_ARG_timezone,
  STRUCT_ARG_itimerspec,
  STRUCT_ARG_itimerval,
  STRUCT_ARG_sigevent,
  STRUCT_ARG_stat64,
  STRUCT_ARG_rlimit64,
  STRUCT_ARG_msghdr,
  STRUCT_ARG_iovec,
  STRUCT_ARG_mmsghdr,
  STRUCT_ARG_tms,
  STRUCT_ARG_epoll_even,
  STRUCT_ARG_getcpu_cache,
  STRUCT_ARG_rlimi,
  STRUCT_ARG_iocb,
  STRUCT_ARG_perf_event_attr,
  STRUCT_ARG_new_utsname,
  STRUCT_ARG_stat,
  STRUCT_ARG_linux_dirent64,
  STRUCT_ARG_linux_dirent,
  STRUCT_ARG_sembuf,
  STRUCT_ARG_statfs64,
  STRUCT_ARG_shmid_ds,
  STRUCT_ARG_pollfd,
  STRUCT_ARG_msgbu,
  STRUCT_ARG_file_handle,
  STRUCT_ARG_robust_list_head,
  STRUCT_ARG_robust_list,
  STRUCT_ARG_rusage,
  STRUCT_ARG_ustat,
  STRUCT_ARG_timex,
  STRUCT_ARG_statfs,
  STRUCT_ARG_msqid_ds,
  STRUCT_ARG_ipc_per,
  STRUCT_ARG_msg,
  STRUCT_ARG_sysctl_args,
  STRUCT_ARG_mq_attr,
  STRUCT_ARG_io_event,
  STRUCT_ARG_sched_param,
  STRUCT_ARG_kexec_segment,
  STRUCT_ARG_utimbuf,
  STRUCT_ARG_sockaddr,
  STRUCT_ARG_sel_arg_struct,
  STRUCT_ARG_epoll_event,
  STRUCT_ARG_rlimit,
  STRUCT_ARG_msgbuf,
  STRUCT_ARG___sysctl_args,
  STRUCT_ARG_mmap_arg_struct,
  STRUCT_ARG_siginfo

};

struct arglist {
	unsigned int num;
	unsigned long values[32];
};

struct errnos {
	unsigned int num;
	int values[32];
};

struct misc_arg_info {
       enum struct_argtype struct_type;
};

struct syscallentry {
	void (*sanitise)(struct syscallrecord *rec);
	void (*post)(struct syscallrecord *rec);
	int (*init)(void);
	char * (*decode)(struct syscallrecord *rec, unsigned int argnum);

	unsigned int number;
	unsigned int active_number;
	const char name[80];
	const unsigned int num_args;
	unsigned int flags;

	const enum argtype arg1type;
	const enum argtype arg2type;
	const enum argtype arg3type;
	const enum argtype arg4type;
	const enum argtype arg5type;
	const enum argtype arg6type;

	const char *arg1name;
	const char *arg2name;
	const char *arg3name;
	const char *arg4name;
	const char *arg5name;
	const char *arg6name;

        struct misc_arg_info arg1misc;
        struct misc_arg_info arg2misc;
        struct misc_arg_info arg3misc;
        struct misc_arg_info arg4misc;
        struct misc_arg_info arg5misc;
        struct misc_arg_info arg6misc;
  
	/* FIXME: At some point, if we grow more type specific parts here,
	 * it may be worth union-ising this
	 */

	/* ARG_RANGE */
	const unsigned int low1range, hi1range;
	const unsigned int low2range, hi2range;
	const unsigned int low3range, hi3range;
	const unsigned int low4range, hi4range;
	const unsigned int low5range, hi5range;
	const unsigned int low6range, hi6range;

	/* ARG_OP / ARG_LIST */
	const struct arglist arg1list;
	const struct arglist arg2list;
	const struct arglist arg3list;
	const struct arglist arg4list;
	const struct arglist arg5list;
	const struct arglist arg6list;

	const unsigned int group;
	const int rettype;

	struct errnos errnos;
};

#define RET_BORING		-1
#define RET_NONE		0
#define RET_ZERO_SUCCESS	1
#define RET_FD			2
#define RET_KEY_SERIAL_T	3
#define RET_PID_T		4
#define RET_PATH		5
#define RET_NUM_BYTES		6
#define RET_GID_T		7
#define RET_UID_T		8

#define GROUP_NONE	0
#define GROUP_VM	1
#define GROUP_VFS	2

struct syscalltable {
	struct syscallentry *entry;
};

#define AVOID_SYSCALL		(1<<0)
#define NI_SYSCALL		(1<<1)
#define BORING			(1<<2)
#define ACTIVE			(1<<3)
#define TO_BE_DEACTIVATED	(1<<4)
#define NEED_ALARM		(1<<5)
#define EXTRA_FORK		(1<<6)
#define IGNORE_ENOSYS		(1<<7)

void do_syscall(struct syscallrecord *rec);
void handle_syscall_ret(struct syscallrecord *rec);

#define for_each_arg(i) \
	for (i = 1; i <= entry->num_args; i++)

