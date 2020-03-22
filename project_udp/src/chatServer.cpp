#include "server.hpp"
#include "msgPool.hpp"

int main(int argc, char* argv[]){
  if(argc < 3){
    perror("ip port error");
    return 0;
  }
  Server chatServ;
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  chatServ.initServer(ip, port);
  chatServ.start();
  while(1){
    sleep(100);
  }
  return 0;
}
