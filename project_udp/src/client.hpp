#pragma once 
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RECV_THREAD_NUM 2;
#define BUF_SIZE 1024
//接收服务器发来的数据
//给服务器发送数据
class Client{
  public:
    Client(){
      _socketfd = -1;
    }
    void create(std::string& ip, uint16_t port){
      //创建套接字
      _socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      //创建线程进行接收发送消息
      int ret = pthread_create(&_pid, NULL, sendStart, (void*)this);
      if(ret < 0){
        perror("sendthread create");
      }
      int i =RECV_THREAD_NUM;
      while(i--){
        int ret = pthread_create(&_pid, NULL, recvStart, (void*)this);
        if(ret < 0){
          perror("recvthread create");
        }
      }
      //ip 和 port 
      _ip = ip;
      _port = port;
    }

    static void* sendStart(void* arg){
      pthread_detach(pthread_self());
      Client* cli = (Client*)arg;
      while(1){
        cli->sendMessage();
      }
    }
    static void* recvStart(void* arg){
      pthread_detach(pthread_self());
      Client* cli = (Client*)arg;
      while(1){
        cli->recvMessage();
      }
    }
    //收发数据
    void sendMessage(){
      std::string msg;
      printf("client say :");
      fflush(stdout);
      std::cin >> msg;
      sockaddr_in destAddr;
      destAddr.sin_family = AF_INET;
      destAddr.sin_port = htons(_port);
      destAddr.sin_addr.s_addr = inet_addr(_ip.c_str());
      int ret = sendto(_socketfd, msg.c_str(), msg.size(), 0, (struct sockaddr*)&destAddr, sizeof(destAddr));
      fflush(stdout);
      if(ret < 0){
        printf("sendMessage failed\n");
        perror("sendMessage :");
      }
      printf("sendMessage success\n");
    }
    void recvMessage(){
      sockaddr_in destAddr;
      socklen_t len = sizeof(destAddr);
      char buf[BUF_SIZE] = {0};
      int ret = recvfrom(_socketfd, buf, sizeof(buf), 0, (struct sockaddr*)&destAddr, &len);
      if(ret < 0){
        perror("recvMessage :");
      }
      else{
        std::string data;
        data.assign(buf, ret);
        printf("boss say :%s\n", data.c_str());
      }
    }
  private:

    int _socketfd;
    pthread_t _pid;
    //服务器的ip 和port
    std::string _ip;
    uint16_t _port;
};
