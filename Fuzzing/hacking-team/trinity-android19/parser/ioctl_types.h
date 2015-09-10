
struct venc_ioctl_msg {
  void *in;
  void *out;
};


struct qseecom_register_listener_req {
  uint32_t listener_id;
  int32_t ifd_data_fd;
  uint32_t virt_sb_base;
  uint32_t sb_size;
};

struct qseecom_send_cmd_req {
  void *cmd_req_buf;
  int cmd_req_len; 
  void *resp_buf; 
  int resp_len; 
};

struct qseecom_ion_fd_info {
  int32_t fd;
  uint32_t cmd_buf_offset;
};

struct qseecom_send_modfd_cmd_req {
  void *cmd_req_buf; 
  int cmd_req_len; 
  void *resp_buf; 
  int resp_len; 
  struct qseecom_ion_fd_info ifd_data[4];
};


struct qseecom_send_resp_req {
  void *resp_buf; 
  int resp_len; 
};


struct qseecom_load_img_req {
  uint32_t mdt_len; 
  uint32_t img_len; 
  int32_t  ifd_data_fd; 
  char img_name[32]; 
  int app_id; 
};

struct qseecom_set_sb_mem_param_req {
  int32_t ifd_data_fd; 
  uint32_t virt_sb_base;
  uint32_t sb_len;
};

struct qseecom_qseos_version_req {
  int qseos_version;
};


struct qseecom_qseos_app_load_query {
  char app_name[32]; 
  int app_id; 

};

struct kgsl_devinfo {

  int device_id;
  int chip_id;
  int mmu_enabled;
  int gmem_gpubaseaddr;
  int gpu_id;
  int gmem_sizebytes;
};


struct kgsl_devmemstore {
  int soptimestamp;
  int sbz;
  int eoptimestamp;
  int sbz2;
  int ts_cmp_enable;
  int sbz3;
  int ref_wait_ts;
  int sbz4;
  int current_context;
  int sbz5;
};


struct kgsl_shadowprop {
  int gpuaddr;
  int size;
  int flags;
};

struct kgsl_version {
  int drv_major;
  int drv_minor;
  int dev_major;
  int dev_minor;
};


struct kgsl_ibdesc {
  int gpuaddr;
  void *hostptr;
  int sizedwords;
  int ctrl;
};


struct kgsl_device_getproperty {
  int type;
  void  *value;
  int sizebytes;
};


struct kgsl_device_waittimestamp {
  int timestamp;
  int timeout;
};


struct kgsl_device_waittimestamp_ctxtid {
  int context_id;
  int timestamp;
  int timeout;
};


struct kgsl_ringbuffer_issueibcmds {
  int drawctxt_id;
  void *ibdesc_addr;
  int numibs;
  int timestamp;
  int flags;
};


struct kgsl_cmdstream_readtimestamp {
  int type;
  int timestamp;
};


struct kgsl_cmdstream_freememontimestamp {
  void *gpuaddr;
  int type;
  int timestamp;
};


struct kgsl_drawctxt_create {
  int flags;
  int drawctxt_id;
};


struct kgsl_map_user_mem {
  int fd;
  void *gpuaddr;  
  int len;
  int offset;
  void *hostptr;
  enum kgsl_user_mem_type memtype;
  int flags;
};


struct kgsl_cmdstream_readtimestamp_ctxtid {
  int context_id;
  int type;
  int timestamp;
};

struct kgsl_cmdstream_freememontimestamp_ctxtid {
  int context_id;
  void *gpuaddr;
  int type;
  int timestamp;
};


struct kgsl_sharedmem_from_pmem {
  int pmem_fd;
  void *gpuaddr;
  int len;
  int offset;
};


struct kgsl_sharedmem_free {
  void *gpuaddr;
};


struct kgsl_cff_user_event {
  unsigned char cff_opcode;
  int op1;
  int op2;
  int op3;
  int op4;
  int op5;
  int __pad[2];
};


struct kgsl_gmem_desc {
  int x;
  int y;
  int width;
  int height;
  int pitch;
};


struct kgsl_buffer_desc {
  void *hostptr;
  void *gpuaddr;
  int   size;
  int format;
  int pitch;
  int enabled;
};

struct kgsl_bind_gmem_shadow {
  int drawctxt_id;
  struct kgsl_gmem_desc gmem_desc;
  int shadow_x;
  int shadow_y;
  struct kgsl_buffer_desc shadow_buffer;
  int buffer_id;
};

struct kgsl_sharedmem_from_vmalloc {
  void *gpuaddr;
  void *hostptr;
  int flags;
};


struct kgsl_drawctxt_set_bin_base_offset {
  int drawctxt_id;
  int offset;
};

struct kgsl_cmdwindow_write {
  enum kgsl_cmdwindow_type target;
  void *addr;
  int data;
};


struct kgsl_cff_syncmem {
  void *gpuaddr;
  int len;
  int __pad[2];
};

struct kgsl_timestamp_event {
  int type; 
  int timestamp;
  int context_id;
  void *priv; 
  size_t len; 
};


struct kgsl_timestamp_event_genlock {
  int handle;
};


struct kgsl_timestamp_event_fence {
  int fence_fd;
};


struct kgsl_gpumem_alloc_id {
  int id;
  int flags;
  int size;
  int mmapsize;
  void  *gpuaddr;
  int __pad[2];
};


struct kgsl_gpumem_free_id {
  int id;
  int __pad;
};


struct kgsl_gpumem_get_info {
  void *gpuaddr;
  int id;
  int flags;
  int size;
  int mmapsize;
  void *useraddr;
  int __pad[4];
};


struct kgsl_gpumem_sync_cache {
  void *gpuaddr;
  int id;
  int op;
  int __pad[2]; 
};

struct kgsl_perfcounter_get {
  int groupid;
  int countable;
  int offset;
  int __pad[2];
};


struct kgsl_perfcounter_put {
  int groupid;
  int countable;
  int __pad[2];
};


struct kgsl_perfcounter_query {
  int groupid;
  int *countables;
  int count;
  int max_counters;
  int __pad[2];
};


struct kgsl_perfcounter_read_group {
  int groupid;
  int countable;
  uint64_t value;
};

struct kgsl_perfcounter_read {
  struct kgsl_perfcounter_read_group *reads;
  int count;
  int __pad[2];
};



struct kgsl_drawctxt_destroy {
  int drawctxt_id;
};


struct kgsl_gpumem_alloc {
  void *gpuaddr;
  size_t size;
  int flags;
};




struct mc_ioctl_init {
uint32_t  nq_offset;
uint32_t  nq_length;
uint32_t  mcp_offset;
uint32_t  mcp_length;
};

struct mc_ioctl_info {
uint32_t  ext_info_id;
uint32_t  state;
uint32_t  ext_info;
};


struct mc_ioctl_map {
size_t      len;
uint32_t      handle;
void *addr;
void *phys_addr;
int reused;
};


struct mc_ioctl_reg_wsm {
voidt *buffer;
uint32_t len;
uint32_t pid;
uint32_t handle;
void *table_phys;
};


struct mc_ioctl_execute {
void *phys_start_addr;
uint32_t length;
};

struct mc_ioctl_resolv_cont_wsm {
uint32_t handle;
void *phys;
uint32_t length;
int32_t fd;
};

struct mc_ioctl_resolv_wsm {
uint32_t handle;
int32_t fd;
void *phys;
};



struct msm_jpeg_ctrl_cmd {
  uint32_t type;
  uint32_t len;
  void     *value;
};


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


struct msm_jpeg_hw_cmd {

  uint32_t type;
  uint32_t n;
  uint32_t offset;
  uint32_t mask;
  uint32_t *pdata;
};

struct msm_jpeg_hw_cmds {
  uint32_t m;
  struct msm_jpeg_hw_cmd hw_cmd;
};



struct timespec {
  long tv_sec;
  long            tv_nsec;
};



struct v4l2_capability {
  __u8    driver[16];
  __u8    card[32];
  __u8    bus_info[32];
  __u32   version;
  __u32   capabilities;
  __u32   device_caps;
  __u32   reserved[3];
};


struct v4l2_fmtdesc {
  __u32               index; 
  enum v4l2_buf_type  type; 
  __u32               flags;
  __u8                description[32];
  __u32               pixelformat;
  __u32               reserved[4];
};


struct v4l2_rect {
  __s32   left;
  __s32   top;
  __s32   width;
  __s32   height;
};



struct v4l2_clip {
  struct v4l2_rect  c;
  void  *next;
};


struct v4l2_window {
  struct v4l2_rect w;
  enum v4l2_field  field;
  __u32 chromakey;
  struct v4l2_clip *clips;
  __u32 clipcount;
  void  *bitmap;
  __u8  global_alpha;
};


struct v4l2_format {
  enum v4l2_buf_type type;
  struct v4l2_window win;  
};


struct v4l2_requestbuffers {
  __u32                   count;
  enum v4l2_buf_type      type;
  enum v4l2_memory        memory;
  __u32                   reserved[2];
};


struct v4l2_timecode {
  __u32   type;
  __u32   flags;
  __u8    frames;
  __u8    seconds;
  __u8    minutes;
  __u8    hours;
  __u8    userbits[4];
};


struct v4l2_plane {
  __u32                   bytesused;
  __u32                   length;
    void   *userptr;
  __u32                   data_offset;
  __u32                   reserved[11];
};


struct timeval {
  long         tv_sec;
  long   tv_usec;
};

struct v4l2_buffer {
  __u32                   index;
  enum v4l2_buf_type      type;
  __u32                   bytesused;
  __u32                   flags;
  enum v4l2_field         field;
  struct timeval          timestamp;
  struct v4l2_timecode    timecode;
  __u32                   sequence;

  enum v4l2_memory        memory;
  struct v4l2_plane *planes;
  __u32                   length;
  __u32                   input;
  __u32                   reserved;
};


struct v4l2_plane_pix_format {
  __u32           sizeimage;
  __u16           bytesperline;
  __u16           reserved[7];
};


struct v4l2_pix_format_mplane {
  __u32                           width;
  __u32                           height;
  __u32                           pixelformat;
  enum v4l2_field                 field;
  enum v4l2_colorspace            colorspace;

  struct v4l2_plane_pix_format    plane_fmt[8];
  __u8                            num_planes;
  __u8                            reserved[11];
};


struct v4l2_pix_format {
  __u32                   width;
  __u32                   height;
  __u32                   pixelformat;
  enum v4l2_field         field;
  __u32                   bytesperline;
  __u32                   sizeimage;
  enum v4l2_colorspace    colorspace;
  __u32                   priv;        
};


struct v4l2_framebuffer {
  __u32                   capability;
  __u32                   flags;
  void                    *base;
  struct v4l2_pix_format  fmt;
};


struct v4l2_streamparm {
  enum v4l2_buf_type type;
  __u8    raw_data[200];
};


struct v4l2_fract {
  __u32   numerator;
  __u32   denominator;
};


struct v4l2_standard {
  __u32                index;
  v4l2_std_id          id;
  __u8                 name[24];
  struct v4l2_fract    frameperiod;
  __u32                framelines;
  __u32                reserved[4];
};


struct v4l2_input {
  __u32        index; 
  __u8         name[32]; 
  __u32        type;
  __u32        audioset;
  __u32        tuner;
  v4l2_std_id  std;
  __u32        status;
  __u32        capabilities;
  __u32        reserved[3];
};


struct v4l2_control {
  __u32                id;
  __s32                value;
};


struct v4l2_tuner {
  __u32                   index;
  __u8                    name[32];
  enum v4l2_tuner_type    type;
  __u32                   capability;
  __u32                   rangelow;
  __u32                   rangehigh;
  __u32                   rxsubchans;
  __u32                   audmode;
  __s32                   signal;
  __s32                   afc;
  __u32                   reserved[4];
};

struct v4l2_modulator {
  __u32                   index;
  __u8                    name[32];
  __u32                   capability;
  __u32                   rangelow;
  __u32                   rangehigh;
  __u32                   txsubchans;
  __u32                   reserved[4];
};


struct v4l2_audio {
  __u32   index;
  __u8    name[32];
  __u32   capability;
  __u32   mode;
  __u32   reserved[2];
};


struct v4l2_queryctrl {
  __u32                id;
  enum v4l2_ctrl_type  type;
  __u8                 name[32];
  __s32                minimum;
  __s32                maximum;
  __s32                step;
  __s32                default_value;
  __u32                flags;
  __u32                reserved[2];
};


struct v4l2_querymenu {
  __u32           id;
  __u32           index;
  __u8            name[32];
  __u32           reserved;
};


struct v4l2_outputparm {
  __u32              capability;
  __u32              outputmode;
  struct v4l2_fract  timeperframe;
  __u32              extendedmode;
  __u32              writebuffers;
  __u32              reserved[4];

};


struct v4l2_audioout {
  __u32   index;
  __u8    name[32];
  __u32   capability;
  __u32   mode;
  __u32   reserved[2];
};




struct v4l2_frequency {
  __u32                 tuner;
  enum v4l2_tuner_type  type;
  __u32                 frequency;
  __u32                 reserved[8];
};




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



struct v4l2_jpegcompression {
  int quality;

  int  APPn;              
			  
  int  APP_len;           
  char APP_data[60];      

  int  COM_len;           
  char COM_data[60];      

  __u32 jpeg_markers;     
  int marker;

			  
};


struct v4l2_sliced_vbi_cap {
  __u16   service_set;
  __u16   service_lines[2][24];
  enum v4l2_buf_type type;
  __u32   reserved[3];
};

struct v4l2_ext_controls {
  __u32 ctrl_class;
  __u32 count;
  __u32 error_idx;
  __u32 reserved[2];
  void *controls;
};

struct v4l2_frmsize_stepwise {
  __u32                   min_width;      
  __u32                   max_width;      
  __u32                   step_width;     
  __u32                   min_height;     
  __u32                   max_height;     
  __u32                   step_height;    
};



struct v4l2_frmsizeenum {
  __u32                   index;          
  __u32                   pixel_format;   
  __u32                   type;           
    struct v4l2_frmsize_stepwise    stepwise;
  __u32   reserved[2];                    
};

struct v4l2_frmival_stepwise {
  struct v4l2_fract       min; 
  struct v4l2_fract       max; 
  struct v4l2_fract       step;
};


struct v4l2_frmivalenum {
  __u32                   index;          
  __u32                   pixel_format;   
  __u32                   width;          
  __u32                   height;         
  __u32                   type;           
    struct v4l2_frmival_stepwise    stepwise;

  __u32   reserved[2]; 
};


struct v4l2_enc_idx_entry {
  __u64 offset;
  __u64 pts;
  __u32 length;
  __u32 flags;
  __u32 reserved[2];
};


struct v4l2_enc_idx {
  __u32 entries;
  __u32 entries_cap;
  __u32 reserved[4];
  struct v4l2_enc_idx_entry entry[64];
};


struct v4l2_encoder_cmd {
  __u32 cmd;
  __u32 flags;      
  __u32 data[8];
};

struct v4l2_dbg_match {
  __u32 type;
  __u32 addr;
  char name[32];
};


struct v4l2_dbg_register {
  struct v4l2_dbg_match match;
  __u32 size;
  __u64 reg;
  __u64 val;
};


struct v4l2_dbg_chip_ident {
  struct v4l2_dbg_match match;
  __u32 ident; 
  __u32 revision; 
};


struct v4l2_hw_freq_seek {
  __u32                 tuner;
  enum v4l2_tuner_type  type;
  __u32                 seek_upward;
  __u32                 wrap_around;
  __u32                 spacing;
  __u32                 reserved[7];
};


struct v4l2_dv_enum_preset {
  __u32   index;
  __u32   preset;
  __u8    name[32];
  __u32   width;
  __u32   height;
  __u32   reserved[4];
};


struct v4l2_dv_preset {
  __u32   preset;
  __u32   reserved[4];
};


struct v4l2_dv_timings {
  __u32 type;
  __u32   reserved[32];
};



struct v4l2_event {
  __u32                           type;
  __u8                            data[64];
  __u32                           pending;
  __u32                           sequence;
  struct timespec                 timestamp;
  __u32                           id;
  __u32                           reserved[8];
};


struct v4l2_event_subscription {
  __u32                           type;
  __u32                           id;
  __u32                           flags;
  __u32                           reserved[5];
};



struct v4l2_create_buffers {
  __u32                   index;
  __u32                   count;
  enum v4l2_memory        memory;
  struct v4l2_format      format;
  __u32                   reserved[8];
};


struct v4l2_selection {
  __u32                   type;
  __u32                   target;
  __u32                   flags;
  struct v4l2_rect        r;
  __u32                   reserved[9];
};


struct v4l2_decoder_cmd {
  __u32 cmd;
  __u32 flags;
  __s32 speed;
  __u32 format;
};


struct v4l2_output {
  __u32        index;     
  __u8         name[32];  
  __u32        type;      
  __u32        audioset;  
  __u32        modulator; 
  int  std;
  __u32        capabilities;
  __u32        reserved[3];
};
