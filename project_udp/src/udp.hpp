#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>


#include "request.hpp"


class udpServer{
  public:
    udpServer(){
      _sock = -1;
    }

    void Close(){
      close(_sock);
    }
    //创建套接字
    bool createudp(){
      _sock = socket(AF_INET, SOCK_DGRAM, 0);
      if(_sock < 0){
        return false;
      }
      return true;
    }

    //绑定信息
    bool Bind(std::string& ip, uint16_t port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);
      int ret = bind(_sock, (sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        perror("bind");
        return false;
      }
      return true;
    }
    //发送
    bool Send(messageInfo message, std::string& ip, uint16_t& port){
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);
      int ret = sendto(_sock, &message, sizeof(message), 0, (sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        perror("send");
        return false;
      }
      return true;
    }

    //接收都是出参
    bool Receive(std::string& str, std::string& ip, uint16_t& port){
      sockaddr_in addr;
      socklen_t len = sizeof(addr);
      char buf[1024 * 4] = {0};
      ssize_t ret = recvfrom(_sock, buf, sizeof(buf) - 1, 0, (sockaddr*)&addr,&len);
      if(ret < 0){
        perror("receive");
        return false;
      }
      str = buf;
      ip = inet_ntoa(addr.sin_addr);
      port = ntohs(addr.sin_port);
      return true;
    }
  private:
    int _sock;
};
