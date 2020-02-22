#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
  int fd = open("t.txt", O_RDWR | O_CREAT, 0777);
  write(fd, "wang", 4);
  char ret[1024] = {0};
  lseek(fd, 0, SEEK_SET);
  read(fd, ret, sizeof(ret)- 1);
  printf("read -> %s\n", ret);
  return 0;
}
