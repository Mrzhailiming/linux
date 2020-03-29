#include "udp.hpp"


int mian(int arg, char* argv[]){
  if(arg != 3){
    perror("参数个数错误");
    return 0;
  }
  std::string ip = argv[1];
  uint16_t port = 1999;
  udp u;
  u.createudp();

  //绑定ip, port
  u.Bind(ip, port);
  while(1){
    std::string ret;
    std::string peerip;
    uint16_t peerport;
    u.Receive(ret, peerip, peerport);
    cout << ret.c_str() << endl;

    cout << "ser:";
    fflush(cin);
    cin >> ret;
    u.Send()
  }

}
