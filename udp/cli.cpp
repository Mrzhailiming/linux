#include "udp.hpp"


int main(){
  udp u;
  u.createudp();
  std::string destip = "192.168.88.128";
  uint16_t destport = 1999;
  while(1){
    std::string send;
    std::cout << "cli:";
    std::cin >> send;
    u.Send(send, destip, destport);
    u.Receive(send, destip, destport);

    std::cout << "ser:" << send.c_str() << std::endl;

  }
  u.Close();
  return 0;
}
