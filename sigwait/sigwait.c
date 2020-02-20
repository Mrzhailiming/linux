#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>


void callBack(int num){
  printf("callBack receive -> %d\n", num);
  wait(NULL);
}

int main(){
  signal(SIGCHLD, callBack);
  int ret = fork();
  if(ret < 0){
    perror("fork error");
    return 0;
  }
  if(ret == 0){
    //子进程
    printf("child exit\n");
    sleep(5);
    exit(1);
  }
  else{
    //父进程
    printf("father\n");
    while(1){
      printf("no\n");
      sleep(1);
    }
  }
  return 0;
}


