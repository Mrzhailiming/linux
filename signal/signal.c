#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int gNum = 0;
void callBack(int num){
  ++gNum;
}

int main(){
  signal(2, callBack);
  int count = 0;
  int x = 10;
  while(x--){
    ++count;
    ++gNum;
    printf("count = %d -- gNum = %d\n", count, gNum);
    sleep(1);
  }


  return 0;
}
