
#include <stdio.h>
#include <stdlib.h>
#include <istream>
#include "udp.hpp"
#include "tcp.hpp"
#include "server.hpp"
#define TCP_PORT 1997
#define UDP_PORT 1999


tcpServ tser;

//注册
void userRegister(){
  //先发一个标志位
  std::string flag;
  flag = "3";
  tser.Send(flag);
  printf("send flag success\n");
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
  //发送请求
  tser.Send(rg);
  printf("发送请求成功\n");
}

//登录
void userLogin(){
  //先发一个标志位
  std::string flag;
  flag = "1";
  tser.Send(flag);
  printf("send flag success\n");
  loginRequest li;
  printf("请输入ID:");
  fflush(stdout);
  std::cin >> li._userId;
  printf("请输入密码:");
  fflush(stdout);
  std::cin >> li._password;
  //发送请求
  tser.Send(li);
  printf("发送请求成功\n");
}


void userLogout(){
  ;
}

void menu(){
  printf("**************************\n");
  printf("***1.登录 2.登出 3.注册***\n");
  printf("**************************\n");

  int choice = 0;
  fflush(stdout);
  std::cin >> choice;
  switch(choice){
    case 1:
      userLogin();
      break;
    case 2:
      userLogout();
      break;
    case 3:
      userRegister();
      break;
    default:
      break;
  }
} 
int main(){
  tser.CreateSocket();
  std::string ip = "192.168.88.128";
  uint16_t port = TCP_PORT;
  tser.ConnetServ(ip, port);
  menu();

  //接收应答
  replyRequest rep;
  printf("recv replyRequest ing\n");
  tser.Recv(rep);
  printf("recv replyRequest success\n");
  if(rep._stat == REGESTER){
    printf("注册成功\n");
    printf("你的id为 : %d\n", rep._userId);
    menu();
  }
  printf("开始聊天了\n");
  udpServer user;
  user.createudp();
  port = UDP_PORT;
  messageInfo sendMessg;
  sendMessg._userId = rep._userId;
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
  return 0;
}
