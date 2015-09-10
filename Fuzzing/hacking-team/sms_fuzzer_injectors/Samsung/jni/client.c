#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

static void write_int(int fd, int val) {
  int written = write(fd, &val, sizeof(int));
  if (written != sizeof(int)) {
    exit(-1);
  }
}


int main(int argc, char *argv[])
{
  int sockfd = 0, n = 0;
 
  char sendBuff[] = "\x07\x91\x44\x87\x20\x00\x30\x23\x24\x0D\xD0\xE4\x74\xD8\x1C\x0E\xBB\x01\x00\x00\x11\x10\x11\x31\x52\x14\x00\x0B\xE4\x74\xD8\x1C\x0E\xBB\x5D\xE3\x77\x1B";
  struct sockaddr_in serv_addr; 

  if(argc != 2)
    {
      printf("\n Usage: %s <ip of server> \n",argv[0]);
      return 1;
    } 

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf("\n Error : Could not create socket \n");
      return 1;
    } 

  memset(&serv_addr, '0', sizeof(serv_addr)); 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8888); 

  if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
      printf("\n inet_pton error occured\n");
      return 1;
    } 

  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  write_int(sockfd, sizeof(sendBuff));

  int w = write(sockfd, sendBuff, sizeof(sendBuff));

  printf("Written %d bytes\n", w);

  close(sockfd);

}

