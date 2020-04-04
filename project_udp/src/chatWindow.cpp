#include "chatWindow.hpp"
#include "clientServer.hpp"

int main(){
  chatWindow cw;
  clientServer  cli;
  cw.startWindow(&cli);

  while(1){
    sleep(10);
  }
  return 0;
}
