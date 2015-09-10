#include <termios.h>

/* memory map for libraries */
#define MAX_NAME_LEN 256
#define MEMORY_ONLY  "[memory]"
struct mm {
	char name[MAX_NAME_LEN];
	unsigned long start, end;
};

int find_name(pid_t pid, char *name, char *libn, unsigned long *addr);
int load_memmap(pid_t pid, struct mm *mm, int *nmmp);

int tcgetattr(int fd, struct termios *termios_p);
int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);

void cfmakeraw(struct termios *t);
