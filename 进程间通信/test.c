#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
  int fd = open("t.txt", O_RDWR);
  int count = 0;
  if(fd < 0){
    perror("open error");
    return 0;
  }
  while(1){
    ++count;
    write(fd, "test", 4);
    printf("write success -- %d\n", count);
    sleep(1);
  }
  return 0;
}
