#include <unistd.h>
#include <stdio.h>
//管道
//wangjiaxushizhu
int main(){
  int fd[2];
  int ret = pipe(fd);
  if(ret == -1){
    return 0;
  }
  ret = fork();
  if(ret < 0){
    perror("fork perror:");
  }
  else if(ret == 0){
    //子进程
    //读
    char buf[1024] = {0};
    read(fd[0], buf, sizeof(buf) - 1);
    printf("读 : %s\n",buf);
    while(1){
      sleep(1);
    }
  }else{
    //父进程
    write(fd[1], "dadou", 5);
    while(1){
      sleep(1);
    }
  }
  

  return 0;
}
