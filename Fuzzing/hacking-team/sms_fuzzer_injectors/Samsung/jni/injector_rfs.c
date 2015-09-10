#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/system_properties.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sec-ril.h"

static int read_int(int fd) {
  int val;
  int len = read(fd, &val, sizeof(int));
  if (len != sizeof(int)) {
    exit(-1);
  }
  return val;
}


// Library injection

void __attribute__ ((constructor)) my_init(void);

void my_init(void) {
  void *handle;
  void *reader;
  char recvBuff[1025];
  char *ipc = NULL;

  int listenfd = 0, connfd = 0, n = 0, len = 0;
  struct sockaddr_in serv_addr;
  

  // Header for the ipc message to be queued
  char header[] = "\x30\x00\x00\x00\x04\x02\x03\x02\x01\xff\xff\x12\x1e";

  // Open a socket to get fuzzed PDUs
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(recvBuff, '0', sizeof(recvBuff)); 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(8888); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

  listen(listenfd, 10); 

  while(1) {
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

    // Get the pdu size
    len = read_int(connfd);
    
    ipc = malloc(len + sizeof(header));

    // Get the pdu
    n = read(connfd, recvBuff, sizeof(recvBuff)-1);

    // Concat header + PDU
    memcpy(ipc, header, sizeof(header)-1);
    memcpy((void *) (ipc + sizeof(header) - 1), recvBuff, len);

    // Enqueue the ipc (unsolicited sms received)
    //processIPC((unsigned int)Reader, ipc, len + sizeof(header) - 1);
    processRFS((unsigned int)Reader, ipc, 30 /*len + sizeof(header) - 1*/);
  }
}
