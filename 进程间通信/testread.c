#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){
  int fd = open("t.txt", O_RDWR);
  char buf[1024] = {0};
  int count = 0;
  while(1){
    ++count;
    memset(buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf) - 1);
    printf("read success : %s --- %d\n", buf, count);
  }
  read(fd, buf, sizeof(buf) - 1);
  printf("read success : %s\n", buf);
  return 0;
}
