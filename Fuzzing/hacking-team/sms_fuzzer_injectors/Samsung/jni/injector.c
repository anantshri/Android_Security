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

struct ipc_sms_incoming_msg {
  unsigned char msg_type;
  unsigned char type;
  unsigned short sim_index;
  unsigned char msg_tpid;
  unsigned char length;
} __attribute__((__packed__));


struct ipc_fmt_header {
  unsigned short length;
  unsigned char mseq;
  unsigned char aseq;
  unsigned char group;
  unsigned char index;
  unsigned char type;
} __attribute__((__packed__));



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
  char msg_tpid = 0;


  int listenfd = 0, connfd = 0, n = 0, pdu_len = 0;
  struct sockaddr_in serv_addr;
  struct ipc_fmt_header header;
  struct ipc_sms_incoming_msg sms_incoming;

  // Header for the ipc message to be queued
  //char header[] = "\x64\x00\xff\x56\x04\x02\x03\x02\x01\xff\xff\x12\x1e";

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
    pdu_len = read_int(connfd);
    msg_tpid += 2;

    // Create the ipc header
    header.length = (short) (pdu_len -1) + sizeof(header) + sizeof(sms_incoming);
    header.mseq = (char) 0xff;
    header.aseq = (char) 0x56;
    header.group = (char) 0x4;
    header.index = (char) 0x2;
    header.type = (char) 0x3;

    // Create the ipc request
    sms_incoming.msg_type = (char) 0x2;
    sms_incoming.type = (char) 0x1;
    sms_incoming.sim_index = (short) 0xffff;
    sms_incoming.msg_tpid = (char) msg_tpid;
    sms_incoming.length = (char) pdu_len - 1;

    ipc = malloc(pdu_len + sizeof(header) + sizeof(sms_incoming));

    // Get the pdu
    n = read(connfd, recvBuff, sizeof(recvBuff)-1);

    // Concat header + +sms_incoming + PDU
    memcpy(ipc, &header, sizeof(header));
    memcpy((void *) (ipc + sizeof(header)), &sms_incoming, sizeof(sms_incoming));
    memcpy((void *) (ipc + sizeof(header) +  sizeof(sms_incoming)), recvBuff, pdu_len);

    // Enqueue the ipc (unsolicited sms received)
    processIPC((unsigned int)Reader, ipc, (pdu_len - 1) + sizeof(header) + sizeof(sms_incoming));
  }
}
