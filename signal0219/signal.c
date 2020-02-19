#include <stdio.h>
#include <signal.h>



void callBack(int num){
  printf("signum -- >%d\n", num);
}

int main(){
  //自定义的处理方式
  //9号信号(kill -9)不可以被修改19号也是
  signal(2, callBack);
  while(1){
    ;
  }
  return 0;
}

