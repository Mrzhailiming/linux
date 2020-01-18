#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
  printf("write sucess !");
  //打开之前就可以调用
  int fd = open("t.txt", O_WRONLY);
  int count = 0;
  if(fd < 0){
    perror("file error:");
  }
  while(1){
    ++count;
    write(fd, "wangjiaxu", 9);
    printf("write sucess ! --- %d\n", count);
    sleep(1);
  }  
  return 0;
}
