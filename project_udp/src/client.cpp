#include "client.hpp"
#include <stdlib.h>


int main(int argc, char* argv[]){
  if(argc != 3){
    printf("ip port");
    return 0;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  Client cli;
  cli.create(ip, port);
  while(1){
    sleep(100);
  }
}
