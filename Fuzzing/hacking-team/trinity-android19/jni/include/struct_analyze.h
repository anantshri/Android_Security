#include "syscall.h"
#include <libxml/xmlwriter.h>

extern int nullfd;

#ifdef LOLLIPOP

struct file_handle {
  unsigned int  handle_bytes;   /* Size of f_handle [in, out] */
  int           handle_type;    /* Handle type [out] */
  unsigned char f_handle[0];    /* File identifier (sized by
				   caller) [out] */
};

struct mmap_arg_struct {
  unsigned long addr;
  unsigned long len;
  unsigned long prot;
  unsigned long flags;
  unsigned long fd;
  unsigned long offset;
}; 

struct linux_dirent64 {
  u64             d_ino;
  unsigned long             d_off;
  unsigned short  d_reclen;
  unsigned char   d_type;
  char            d_name[0];
};

struct linux_dirent {
  u32 d_ino;
  unsigned long d_off;
  unsigned short len;
  char d_name[256];
};


struct sel_arg_struct {
  unsigned long n;
  fd_set __user *inp, *outp, *exp;
  struct timeval __user *tvp;
};


struct getcpu_cache {
  unsigned long blob[128 / sizeof(long)];
};

#endif

void struct_recursive_analyze(void *, bool, enum ioctl_struct_type, enum struct_argtype, xmlTextWriterPtr);
