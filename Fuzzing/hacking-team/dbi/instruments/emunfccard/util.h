#include <termios.h>
int find_name(pid_t pid, char *name, char *libn, unsigned long *addr);

int tcgetattr (int fd, struct termios *termios_p);
int tcsetattr (int fd, int optional_actions, const struct termios *termios_p);

void cfmakeraw (struct termios *t);
