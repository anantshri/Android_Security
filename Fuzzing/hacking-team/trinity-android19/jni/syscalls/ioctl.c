/*
 * SYSCALL_DEFINE3(ioctl, unsigned int, fd, unsigned int, cmd, unsigned long, arg)
 */
#include <stdlib.h>
#include <linux/ioctl.h>
#include <linux/major.h>
#include "child.h"
#include "ioctls.h"
#include "maps.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"
#include "syscall.h"
#include "trinity.h"
#include "fuzz_ioctl_struct.h"

static void ioctl_mangle_cmd(struct syscallrecord *rec)
{
	unsigned int i;

	/* mangle the cmd by ORing up to 4 random bits */
	for (i=0; i < (unsigned int)(rand() % 4); i++)
		rec->a2 |= 1L << (rand() % 32);

	/* mangle the cmd by ANDing up to 4 random bits */
	for (i=0; i < (unsigned int)(rand() % 4); i++)
		rec->a2 &= 1L << (rand() % 32);
}

static void ioctl_mangle_arg(struct syscallrecord *rec)
{
  /* the argument could mean anything, because ioctl sucks like that. */
  int p = rand() % 100;
  
  // Rand integer
  if(p < 8) {
    rec->a3 = rand32();
    return;
  }

  // Real mapped addr
  if(p < 25) {
    rec->a3 = (unsigned long) get_non_null_address();
    return;
  }

  // Correct fuzzed struct type
  fuzz_ioctl_struct_type(rec);
  return;



}

static void generic_sanitise_ioctl(struct syscallrecord *rec)
{
	if ((rand() % 50)==0)
	  ioctl_mangle_cmd(rec);

	ioctl_mangle_arg(rec);
}

static void sanitise_ioctl(struct syscallrecord *rec)
{
	const struct ioctl_group *grp;

	if (rand() % 100 == 0) {
	  grp = get_random_ioctl_group();
	}
	else {
	  grp = find_ioctl_group(rec->a1);
	}

	if (grp) {

		grp->sanitise(grp, rec);			  
		ioctl_mangle_arg(rec);

		if (rand() % 100 == 0)
			ioctl_mangle_cmd(rec);
	} else
		generic_sanitise_ioctl(rec);
}

struct syscallentry syscall_ioctl = {
	.name = "ioctl",
	.num_args = 3,
	.arg1name = "fd",
	.arg1type = ARG_FD,
	.arg2name = "cmd",
	.arg3name = "arg",
	.arg3type = ARG_ADDRESS,
	.sanitise = sanitise_ioctl,
	.flags = NEED_ALARM | IGNORE_ENOSYS,
};
