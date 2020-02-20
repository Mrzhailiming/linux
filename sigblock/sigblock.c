#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void callBack(int num){
  printf("callBack --> %d\n", num);
}

int main(){
  signal(2, callBack);
  signal(40, callBack);
  //把sigset_t类型全部置为0
  //sigemptyset(&set);
  //全部置为1
  //sigfillset(&set);
  sigset_t set;
  sigset_t oldset;
  sigemptyset(&set);
  sigemptyset(&oldset);
  //sigprocmask(SIG_BLOCK, &set, &oldset);
  
  sigfillset(&set);
  //阻塞设置
  sigprocmask(SIG_BLOCK, &set, &oldset);
  printf("阻塞\n");
  getchar();

  //非阻塞设置
  sigprocmask(SIG_UNBLOCK, &set, NULL);
  printf("非阻塞设置\n");
  int count = 0;
  while(1){
    ++count;
    printf("while -> %d\n", count);
    sleep(1);
  }
  return 0;
}





