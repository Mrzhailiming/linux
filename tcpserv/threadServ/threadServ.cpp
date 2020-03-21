#include "tcp.hpp"
#include <pthread.h>

void* startFun(void* arg){
  pthread_detach(pthread_self());
  tcpServ* emp = (tcpServ*)arg;
  //线程分离
  while(1){
    std::string ret;
    emp->Recv(ret);
    printf("client send : %s\n", ret.c_str());
    
    printf("you say : ");
    fflush(stdout);
    std::cin >> ret;
    emp->Send(ret);
  }
}

int main(){
  std::string ip = "192.168.88.128";
  uint16_t port = 1999;
  tcpServ boss;
  if(!boss.CreateSocket()){
    return 0;
  }
  if(!boss.BindIpPort(ip, port)){
    return 0;
  }
  if(!boss.Listen(5)){
    return 0;
  }
  while(1){
    tcpServ* emp = new tcpServ;
    boss.Accept(emp);
    printf("accept success!\n");
    //创建线程
    pthread_t th;
    int ret = pthread_create(&th, NULL, startFun, (void*)emp);
    if(ret < 0){
      perror("create pthread error");
      continue;
    }
  }
  return 0;
}


