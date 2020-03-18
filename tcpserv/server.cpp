#include "tcp.hpp"



int main(int argc, char* argv[]){
  if(argc != 3){
    printf("参数错误\n");
    return 0;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  
  tcpServ tser;
  //创建套接字失败
  if(tser.CreateSocket() == false){
    return 0;
  }
  //绑定信息失败
  if(tser.BindIpPort(ip, port) == false){
    return 0;
  }
  //监听
  if(tser.Listen() == false){
    return 0;
  }
  //获取连接
  tcpServ out;
  if(tser.Accept(out) == false){
    return 0;
  }
  printf("tcpServ get a new conncet\n");
  while(1){
    std::string ret;
    out.Recv(ret);
    printf("client: %s\n", ret.c_str());
    
    printf("tcpServ: ");
    fflush(stdout);
    std::cin >> ret;
    out.Send(ret);
  }
  return 0;
}
