#pragma once
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include <map>
#include <set>
#define THREAD_NUM 4
#define RECV_BUF_SIZE 1024
#include "msgPool.hpp"


class Server{
  public:
    Server(){
      _sockfd = -1;
      _messagePool = NULL;
    }
    void initServer(std::string& ip, uint16_t port = 1999){
      //创建套接字
      _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if(_sockfd < 0){
        perror("create socket error");
        exit(1);
      }
      //绑定地址信息
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        perror("bind error");
        exit(2);
      }
      //创建数据池
      _messagePool = new msgPool;
      if(_messagePool == NULL){
        perror("_messagePool create error");
        exit(3);
      }
    }
    //创建线程收发数据
    void start(){
      //创建线程
      for(int i = 0; i < THREAD_NUM; ++i){
        int ret = pthread_create(&th, NULL, pushStart, (void*)this);
        if(ret < 0){
          perror("thread create error");
        }
        ret = pthread_create(&th, NULL, popStart, (void*)this);
      }
    }
    static void* pushStart(void* arg){
      Server* ser = (Server*)arg;
      pthread_detach(pthread_self());
      while(1){
        //一直收数据
        ser->recvMessageFromClient();
      }
      return NULL;
    }
    static void* popStart(void* arg){
      Server* ser = (Server*)arg;
      pthread_detach(pthread_self());
      while(1){
        //一直发数据
        ser->sendMessageToAll();
      }
    }
    ~Server(){
      if(_messagePool){
        delete _messagePool;
      }
    }
  private:
    void recvMessageFromClient(){
      sockaddr_in dest;
      socklen_t len = sizeof(dest);
      char buf[RECV_BUF_SIZE] = {0};
      int ret = recvfrom(_sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&dest, &len);
      if(ret < 0){
        perror("sendto : recvMessageFromClient error");
      }
      else{
        //接收成功, 把message放到数据池 中
        printf("接收成功 并把数据放到数据池中\n");
        std::string data;
        data.assign(buf, ret);
        _messagePool->pushMsg(data);
        //把客户端地址信息存放到set中
        //_clientsAddr.insert(dest);
        std::string ip = inet_ntoa(dest.sin_addr);
        uint16_t port = ntohs(dest.sin_port);
        _clientsAddr.insert(make_pair(ip, port));
        printf("pthread[%d] 客户端添加成功!\n", pthread_self());
      }
    }

    void sendMessageToAll(){
      //获取数据
      std::string data;
      _messagePool->popMsg(data);
      //给所有客户端发送数据
      //获取每一个客户端的地址信息
      //auto it = _clientsAddr.begin();
      //while(it != _clientsAddr.end()){
      //  sendMessageToOne(data, *it, sizeof(sockaddr_in));
      //}
      std::map<std::string, uint16_t>::iterator it = _clientsAddr.begin();
      int count = 1;
      while(it != _clientsAddr.end()){
        sendMessageToOne(data, it->first, it->second);
        printf("向第 %d 个客户端发送 ip: %s port : %d\n",count,  it->first.c_str(), it->second);
        ++count;
        ++it;
      }
    }
    void sendMessageToOne(std::string& data, std::string ip, uint16_t port){
      sockaddr_in destAddr;
      socklen_t destLen = sizeof(destAddr);
      destAddr.sin_family = AF_INET;
      destAddr.sin_port = htons(port);
      destAddr.sin_addr.s_addr = inet_addr(ip.c_str());
      //给一个客户端发送数据
      int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&destAddr, destLen);
      if(ret < 0){
        perror("sendto : sendMessageToOne error");
      }
    }
  private:
    //socket句柄
    int _sockfd;
    pthread_t th;
    //数据池
    msgPool* _messagePool;
    //每个客户端的地址信息
    std::multimap<std::string, uint16_t> _clientsAddr;
};
