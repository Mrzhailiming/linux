#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include "request.hpp"

class tcpServ{
  public:
    tcpServ(){
      _sockfd = -1;
    }
    //创建套接字
    bool CreateSocket(){
      _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_sockfd < 0){
        perror("socket create error");
        return false;
      }
      return true;
    }
    //绑定地址信息端口
    bool BindIpPort(std::string ip, uint16_t port){
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        perror("bind error");
        return false;
      }
      return true;
    }
    //监听
    bool Listen(int num = 5){
      int ret = listen(_sockfd, num);
      if(ret < 0){
        perror("listen error");
        return false;
      }
      return true;
    }
    //连接
    bool ConnetServ(std::string& ip, uint16_t port){
      struct sockaddr_in dest;
      dest.sin_family = AF_INET;
      dest.sin_port = htons(port);
      dest.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = connect(_sockfd, (struct sockaddr*)&dest, sizeof(dest));
      if(ret < 0){
        perror("connet error");
        return false;
      }
      return true;
    }
    //获取连接
    bool Accept(tcpServ& out){
      struct sockaddr_in dest;
      socklen_t destlen = sizeof(dest);
      int ret = accept(_sockfd, (struct sockaddr*)&dest, &destlen);
      if(ret < 0){
        perror("accept error");
        return false;
      }
      out._sockfd = ret;
      return true;
    }
    
    //发送
    bool Send(std::string& buf){
      int ret = send(_sockfd, buf.c_str(), buf.size(), 0);
      if(ret < 0){
        perror("send error");
        return false;
      }
      return true;
    }
    bool Send(registerRequest& rr){
      int ret = send(_sockfd, &rr, sizeof(rr), 0);
      if(ret < 0){
        perror("send error");
        return false;
      }
      return true;
    }
    bool Send(loginRequest& li){
      int ret = send(_sockfd, &li, sizeof(li), 0);
      if(ret < 0){
        perror("send error");
        return false;
      }
      return true;
    }
    bool Send(logoutRequest& lo){
      int ret = send(_sockfd, &lo, sizeof(lo), 0);
      if(ret < 0){
        perror("send error");
        return false;
      }
      return true;
    }
    //接收
    bool Recv(std::string& out){
      char buf[1024] = {0};
      int ret = recv(_sockfd, buf, sizeof(buf) - 1, 0);
      if(ret < 0){
        perror("recv error");
        return false;
      }
      if(ret == 0){
        printf("recv 0 bit");
      }
      out.assign(buf);
      return true;
    }
    bool Recv(replyRequest& rp){

      int ret = recv(_sockfd, &rp, sizeof(rp), 0);
      if(ret < 0){
        perror("recv replyRequest failed");
        return false;
      }
      return true;
    }
  private:
    int _sockfd;
};
