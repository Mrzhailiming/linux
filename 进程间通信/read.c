#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
  int fd = open("t.txt", O_RDWR);
  if(fd < 0){
    perror("read error :");
    return 0;
  }
  char buf[1024] = {0};
  //读取
  read(fd, buf, sizeof(buf) - 1);
  //打印
  printf("read is --> %s\n", buf);
  //while(1){
  //  sleep(1);
  //}
  return 0;
}
