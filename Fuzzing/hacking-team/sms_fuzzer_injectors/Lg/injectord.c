#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <poll.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>

/*
 *  Author: Collin Mulliner <collin@mulliner.org>
 *
 *  web: http://www.mulliner.org/security/sms/
 *
 *  License: GPLv3
 *
 */


#define SMD0 "/dev/pts1"
#define SMD0REAL "/dev/pts1real"

#define offsetof(TYPE, MEMBER) ((int)&((TYPE *)0)->MEMBER)


static FILE *log;
static int dolog = 1;

static int nss = -1;
static int nsc = -1;
static int smd_rild_c;
static int smdreal;

static int sms_avail = 0;
static int got_at = 0;

static int open_log()
{
	if (dolog == 0)
		return 0;
	
	log = fopen("/sdcard/injector.log", "w");

	if (log == NULL) {
		printf("can't open log file\n");
		exit(0);
	}
	return 1;
}

int setup_netsock(int *s, int port)
{
	int len;
	struct sockaddr_in saun ={0};

	*s = socket(AF_INET, SOCK_STREAM, 0);	
	saun.sin_family = AF_INET;
	saun.sin_port = htons(port);
	len = sizeof(saun);
	if (bind(*s, &saun, len) < 0) {
		if (dolog) fprintf(log, "net bind failed\n");
		return -1;
	}
	if (listen(*s, 2) < 0) {
		if (dolog) fprintf(log, "net listen failed\n");
		return -2;
	}
	return 1;
}

static int config_fd(char *name)
{
	int fd = open(name,O_RDWR);
	struct termios  ios;
	tcgetattr(fd, &ios);
	cfmakeraw(&ios);
	ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
	tcsetattr(fd, TCSANOW, &ios);
	
	close(fd);
}

int main(int argc, char **argv)
{
	struct pollfd pfd[3];
	int first = 1;
	
	smd_rild_c = posix_openpt(O_RDWR|O_NOCTTY);
	if (smd_rild_c <= 0)
		printf("posix_openpt failed\n");
	if (unlockpt(smd_rild_c) < 0)
		printf("unlockpt failed\n");
	//printf("fake serial device name: %s\n", ptsname(smd_rild_c));

	struct termios  ios;
	tcgetattr(smd_rild_c, &ios);
	ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
	tcsetattr(smd_rild_c, TCSANOW, &ios);

	smdreal = open(SMD0REAL, O_RDWR);
	if (smdreal < 0) {
		printf("please rename /dev/smd0 to /dev/smd0real before running injectord\n");
		exit(0);
	}
	
	int child = fork();
	if (child == 0) {
		config_fd(ptsname(smd_rild_c));
		exit(0);
	}
	
	remove(SMD0);
	symlink(ptsname(smd_rild_c), SMD0);
	chmod(ptsname(smd_rild_c), 00666);

	open_log();
	
	setup_netsock(&nss, 8888);
	
	for (;;) {
		pfd[0].fd = smd_rild_c;
		pfd[0].events = POLLIN;
		pfd[1].fd = smdreal;
		pfd[1].events = POLLIN;
		if (nsc != -1) {
			pfd[2].fd = nsc;
			pfd[2].events = POLLIN;
		}
		else if (nss != -1) {
			pfd[2].fd = nss;
			pfd[2].events = POLLIN;
		}
		
		fflush(log);
		
		int pfn = poll(pfd, 3, -1);
		
		if (pfd[2].revents & POLLIN == POLLIN && pfd[2].fd == nss) {
			nsc = accept(nss, NULL, NULL);
			if (nsc < 0) {
				nsc = -1;
			}
			else {
				if (dolog) fprintf(log, "fuzz network connected\n");
			}
		}
		else if (pfd[2].revents & POLLIN == POLLIN && pfd[2].fd == nsc) {
			char buffer[4096] = {0};
			int rb = read(nsc, buffer, sizeof(buffer));
			if (rb <= 0) {
				close(nsc);
				nsc = -1;
				if (dolog) fprintf(log, "fuzz network disconnected\n");
			}
			else {
				if (dolog) {
					fprintf(log, "write %d fuzz bytes to smd_rild_c\n", rb);
					fprintf(log, "---\n%s\n+++\n", buffer);
				}
				int wb = write(smd_rild_c, buffer, rb);
				if (wb <= 0) {
					printf("fuzz write failed\n");
				}
				else {
					sms_avail = 1;
					got_at = 0;
				}
			}
		}
		if (pfd[0].revents & POLLIN == POLLIN && pfd[0].fd == smd_rild_c) {
		  if(first) {
		    printf("Opening second fd\n");
		    first = 0;
		    int fd = open(SMD0REAL, O_RDWR);
		  }

			char buffer[1024*8] = {0};
			int rb = read(smd_rild_c, buffer, sizeof(buffer));
			if (rb <= 0) {
				close(smd_rild_c);
				smd_rild_c = -1;
			}
			else {
				if (dolog) {
					fprintf(log, "read %d bytes from smd_rild_c:\n", rb);
					fprintf(log, "---\n%s\n+++\n", buffer);
				}
				if (sms_avail == 1 && got_at == 0) {
					if (dolog) fprintf(log, "swollowing AT after +CMT\n");
					int wb = write(smd_rild_c, "0\r", 2);
					if (wb <= 0) {
						printf("got_at answer failed\n");
					}
					else {
						if (log) fprintf(log, "sms xfer completed\n");
						sms_avail = 0;
						got_at = 0;
					}
				}
				else {
					int wb = write(smdreal, buffer, rb);
					printf("Writing %d to smdreal\n", wb);
					if (wb <= 0) {
					  printf("error writing to smdreal: errno %d\n", errno);
					}
				}
			}
		}
		if (pfd[1].revents & POLLIN == POLLIN) {
			char buffer[1024*8] = {0};
			int rb = read(smdreal, buffer, sizeof(buffer));
			if (rb <= 0) {
				printf("error reading from smdreal\n");
			}
			else {
				if (dolog) {
					fprintf(log, "read %d bytes from smdreal:\n", rb);
					fprintf(log, "+++\n%s\n---\n", buffer);
				}
				
				int wb = write(smd_rild_c, buffer, rb);
				printf("Writing %d to smb_rild\n", wb);
				if (wb <= 0) {
					printf("error writing to smb_rild_c\n");
					close(smd_rild_c);
					smd_rild_c = -1;
				}
			}
		}
	}
}
