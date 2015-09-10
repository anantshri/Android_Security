/*
 *  Collin's Binary Instrumentation Tool/Framework for Android
 *  Collin Mulliner <collin[at]mulliner.org>
 *  http://www.mulliner.org/android/
 *
 *  (c) 2012
 *
 *  License: GPL v2
 *
 */
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

int setup_netsock(int *s, int port)
{
	int len;
	struct sockaddr_in saun ={0};

	*s = socket(AF_INET, SOCK_STREAM, 0);	
	saun.sin_family = AF_INET;
	saun.sin_port = htons(port);
	len = sizeof(saun);
	if (bind(*s, &saun, len) < 0) {
		fprintf(stderr, "net bind failed\n");
		return -1;
	}
	if (listen(*s, 2) < 0) {
		fprintf(stderr, "net listen failed\n");
		return -2;
	}
	return 1;
}

main(int argc, char **argv)
{
	unsigned char buffer[4096];
	
	int nss = -1;
	int nsc = -1;
	
	setup_netsock(&nss, 4223);
	
	for (;;) {
		nsc = accept(nss, NULL, NULL);
		if (nsc < 0) {
			nsc = -1;
		}
		else {
			fprintf(stderr, "network connected\n");
		}

		int rb = read(nsc, buffer, sizeof(buffer));
		if (rb <= 0) {
			close(nsc);
			nsc = -1;
			fprintf(stderr, "client disconnected\n");
			continue;
		}
		
		int fd;
		
		int i;
		for (i = 0; i < 5; i++) {
			char path[100];
			sprintf(path, "/dev/pts/%d", i);
			struct stat st = {0};
			if (stat(path, &st) == 0) {
				if (st.st_uid != 2000 && st.st_uid != 0) {
			
					fd = open(path, O_RDWR);
					if (fd) break;
				}
			}
		}
	
		if (fd < 0) {
			perror("open");
			continue;
		}
		fprintf(stderr, "pty connected\n");
		
		struct termios  ios;
		tcgetattr(fd, &ios);
		cfmakeraw(&ios);
		ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
		tcsetattr(fd, TCSANOW, &ios);
   
   		if (write(fd, buffer, rb) <= 0)
			fprintf(stderr, "pty write failed\n");
   
		close(fd);
	}
}