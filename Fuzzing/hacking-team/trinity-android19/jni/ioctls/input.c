#include <linux/ioctl.h>
#include <linux/input.h>

#include "ioctls.h"
#include "shm.h"
#include "syscall.h"
#include "utils.h"

#define EV_MAX                  0x1f
#define EV_CNT                  (EV_MAX+1)
#define ABS_MAX                 0x3f
#define ABS_CNT                 (ABS_MAX+1)


static const struct ioctl input_ioctls[] = {
  IOCTL(EVIOCGVERSION, 0, 0),
  IOCTL(EVIOCGID, 0, 0),
  IOCTL(EVIOCGREP, 0, 0),
  IOCTL(EVIOCSREP, 0, 0),
  IOCTL(EVIOCGKEYCODE, 0, 0),
#ifdef EVIOCGKEYCODE_V2
  IOCTL(EVIOCGKEYCODE_V2, 0, 0),
#endif
  IOCTL(EVIOCSKEYCODE, 0, 0),
#ifdef EVIOCSKEYCODE_V2
  IOCTL(EVIOCSKEYCODE_V2, 0, 0),
#endif
  IOCTL(EVIOCGNAME(0), 0, 0),
  IOCTL(EVIOCGPHYS(0), 0, 0),
  IOCTL(EVIOCGUNIQ(0), 0, 0),
#ifdef EVIOCGPROP
  IOCTL(EVIOCGPROP(0), 0, 0),
#endif
#ifdef EVIOCGMTSLOTS
  IOCTL(EVIOCGMTSLOTS(0), 0, 0),
#endif
  IOCTL(EVIOCGKEY(0), 0, 0),
  IOCTL(EVIOCGLED(0), 0, 0),
  IOCTL(EVIOCGSND(0), 0, 0),
  IOCTL(EVIOCGSW(0), 0, 0),
  IOCTL(EVIOCGBIT(0,0), 0, 0),
  IOCTL(EVIOCGABS(0), 0, 0),
  IOCTL(EVIOCSABS(0), 0, 0),
  IOCTL(EVIOCSFF, 0, 0),
  IOCTL(EVIOCRMFF, 0, 0),
  IOCTL(EVIOCGEFFECTS, 0, 0),
  IOCTL(EVIOCGRAB, 0, 0),
#ifdef EVIOCSCLOCKID
  IOCTL(EVIOCSCLOCKID, 0, 0),
#endif
};

static const char *const input_devs[] = {
	"input",
};

static void input_sanitise(const struct ioctl_group *grp, struct syscallrecord *rec)
{
	unsigned int u, r;

	pick_random_ioctl(grp, rec);

	switch (rec->a2) {
	case EVIOCGNAME(0):
		u = rand();
		rec->a2 = EVIOCGNAME(u);
		break;
	case EVIOCGPHYS(0):
		u = rand();
		rec->a2 = EVIOCGPHYS(u);
		break;
	case EVIOCGUNIQ(0):
		u = rand();
		rec->a2 = EVIOCGUNIQ(u);
		break;
#ifdef EVIOCGPROP
	case EVIOCGPROP(0):
		u = rand();
		rec->a2 = EVIOCGPROP(u);
		break;
#endif
#ifdef EVIOCGMTSLOTS
	case EVIOCGMTSLOTS(0):
		u = rand();
		rec->a2 = EVIOCGMTSLOTS(u);
		break;
#endif
	case EVIOCGKEY(0):
		u = rand();
		rec->a2 = EVIOCGKEY(u);
		break;
	case EVIOCGLED(0):
		u = rand();
		rec->a2 = EVIOCGLED(u);
		break;
	case EVIOCGSND(0):
		u = rand();
		rec->a2 = EVIOCGSND(u);
		break;
	case EVIOCGSW(0):
		u = rand();
		rec->a2 = EVIOCGSW(u);
		break;
	case EVIOCGBIT(0,0):
		u = rand();
		r = rand();
		if (u % 10) u %= EV_CNT;
		if (r % 10) r /= 4;
		rec->a2 = EVIOCGBIT(u, r);
		break;
	case EVIOCGABS(0):
		u = rand();
		if (u % 10) u %= ABS_CNT;
		rec->a2 = EVIOCGABS(u);
		break;
	case EVIOCSABS(0):
		u = rand();
		if (u % 10) u %= ABS_CNT;
		rec->a2 = EVIOCSABS(u);
		break;
	default:
		break;
	}
}

static const struct ioctl_group input_grp = {
	.devtype = DEV_MISC,
	.devs = input_devs,
	.devs_cnt = ARRAY_SIZE(input_devs),
	.sanitise = input_sanitise,
	.ioctls = input_ioctls,
	.ioctls_cnt = ARRAY_SIZE(input_ioctls),
};

REG_IOCTL_GROUP(input_grp)
