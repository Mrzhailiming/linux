#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
  if(argc != 3){
    perror("ip port");
    return 0;
  }
  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sockfd < 0){
    perror("sokcet error");
    return 0;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip.c_str());
  int ret = bind(sockfd,(struct sockaddr*)&addr, sizeof(addr));
  if(ret < 0){
    perror("bind failed");
    return 0;
  }
  ret = listen(sockfd, 5);
  if(ret < 0){
    perror("listen failed");
    return 0;
  }
  printf("listen ing\n");
  while(1){
    sockaddr_in dest;
    socklen_t len = sizeof(dest);
    int acfd = accept(sockfd, (struct sockaddr*)&dest, &len);
    printf("%d\n", acfd);
    if(acfd < 0){
      perror("accept failed continue");
      continue;
    }
    char buf[1024 * 10] = {0};
    ssize_t ret = recv(acfd, buf, sizeof(buf) - 1, 0);
    if(ret < 0){
      perror("recv error");
      continue;
    }
    printf("recv : %s\n", buf);
    const char* str = "<h1>hello world</h1>";
    char sendBuf[1024] = {0};
    sprintf(sendBuf, "HTTP/1.0 200 OK\nContent-Length:%lu\n\n%s", strlen(str), str);
    send(sockfd, buf, strlen(buf), 0);
  }
  return 0;
}
