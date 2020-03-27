
#include <stdio.h>
#include <stdlib.h>
#include <istream>
#include "udp.hpp"
#include "tcp.hpp"
#include "server.hpp"
#define TCP_PORT 1997
#define UDP_PORT 1999



//注册
void userRegister(uint64_t& userId){
  tcpServ tser;
  tser.CreateSocket();
  std::string ip = "192.168.88.128";
  uint16_t port = TCP_PORT;
  tser.ConnetServ(ip, port);
  //先发一个标志位
  std::string flag;
  flag = "3";
  tser.Send(flag);
  registerRequest rg;
  printf("请输入姓名:");
  fflush(stdout);
  std::cin >> rg._name;
  printf("请输入学校:");
  fflush(stdout);
  std::cin >> rg._school;
  printf("请输入密码:");
  fflush(stdout);
  std::cin >> rg._password;
  //发送登录请求
  tser.Send(rg);

  //接收reply
  replyRequest rp;
  tser.Recv(rp);
  userId = rp._userId;
}

//登录
void userLogin(uint64_t& userId){
  tcpServ tser;
  tser.CreateSocket();
  std::string ip = "192.168.88.128";
  uint16_t port = TCP_PORT;
  tser.ConnetServ(ip, port);
  //先发一个标志位
  std::string flag;
  flag = "1";
  tser.Send(flag);
  loginRequest li;
  printf("请输入ID:");
  fflush(stdout);
  std::cin >> li._userId;
  printf("请输入密码:");
  fflush(stdout);
  std::cin >> li._password;
  //发送登录请求
  tser.Send(li);
  
  //接收reply
  replyRequest rp;
  if(rp._stat == -1){
    printf("登陆失败\n");
    userId = -1;
    return;
  }
  printf("userId 为 : %ld\n", rp._userId);
  userId = rp._userId;
}


//退出
void userLogout(uint64_t& userId){
  tcpServ tser;
  tser.CreateSocket();
  std::string ip = "192.168.88.128";
  uint16_t port = TCP_PORT;
  tser.ConnetServ(ip, port);
  ;
}

uint64_t menu(){
  printf("**************************\n");
  printf("***1.登录 2.登出 3.注册***\n");
  printf("**************************\n");

  uint64_t userId = -1;
  int choice = 0;
  fflush(stdout);
  std::cin >> choice;
  switch(choice){
    case 1:
      userLogin(userId);
      break;
    case 2:
      userLogout(userId);
      break;
    case 3:
      userRegister(userId);
      break;
    default:
      break;
  }
  return userId;
}
void chat(uint64_t userId){
  udpServer user;
  user.createudp();
  std::string ip = "192.168.88.128";
  uint16_t port = UDP_PORT;
  messageInfo sendMessg;
  sendMessg._userId = userId;
  while(1){
    char str[MESSAGE_SIZE] = {0};
    printf("you say:");
    fflush(stdout);
    std::cin >> str;
    memcpy(sendMessg._data, str, sizeof(str));
    user.Send(sendMessg, ip, port);

    std::string recv;
    user.Receive(recv, ip, port);
    printf("server say : %s\n", recv.c_str());
  }
}
int main(){
  uint64_t userId = menu();
  while(userId < 0){
    printf("账号密码错误\n");
    userId = menu();
  }
  chat(userId);
  return 0;
}
