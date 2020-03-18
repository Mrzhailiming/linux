#include "tcp.hpp"


int main(){
  std::string ip = "192.168.88.128";
  uint16_t port = 1999;
  tcpServ cli;
  if(cli.CreateSocket() == false){
    return 0;
  }
  if(cli.ConnetServ(ip, port) == false){
    return 0;
  }
  std::string str;
  while(1){
    printf("client:");
    fflush(stdout);
    std::cin >> str;
    cli.Send(str);
    cli.Recv(str);
    printf("tcpServ: %s\n", str.c_str());
  }
}
