#include "server.hpp"
#include "msgPool.hpp"

int main(){
  Server chatServ;
  chatServ.initServer();
  chatServ.start();
  while(1){
    sleep(100);
  }
  //sleep(10);
  return 0;
}
