#include <stdio.h>
#include <signal.h>
#include <unistd.h>


//struct sigaction {
//  void     (*sa_handler)(int);
//  void     (*sa_sigaction)(int, siginfo_t *, void *);
//  sigset_t   sa_mask;//位图
//  int        sa_flags;
//  void     (*sa_restorer)(void);                                                                                      
//};

void callBack(int signum){
  printf("signum --> %d\n", signum);
}

int main(){
  //入参
  struct sigaction newact;
  newact.sa_handler = callBack;
  //flags 置为0
  newact.sa_flags = 0;
  //清空位图的bit位
  sigemptyset(&newact.sa_mask);

  //出参
  struct sigaction oldact;

  sigaction(SIGINT, &newact, &oldact);
  while(1){
    printf("sigaction -->\n");
    sleep(1);
  }
  return 0;
}
