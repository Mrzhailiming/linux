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

#include "msgPool.hpp"
#include "request.hpp"
#include "userManager.hpp"

#define UDP_PORT 1999
#define TCP_PORT 1997
#define THREAD_NUM 4
#define RECV_BUF_SIZE 1024 * 10

class Server{
  public:
    Server(){
      _sockfd = -1;
      _tcpSocket = -1;
      _messagePool = NULL;
      _userMng = NULL;
    }
    ~Server(){
      if(_messagePool){
        delete _messagePool;
      }
      if(_userMng){
        delete _userMng;
      }
    }
    void initServer(){
      //创建收发数据套接字
      _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if(_sockfd < 0){
        perror("create udpsocket error");
        exit(1);
      }
      //绑定地址信息
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(UDP_PORT);
      addr.sin_addr.s_addr = inet_addr("0.0.0.0");
      int ret = bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        perror("udp bind error");
        exit(2);
      }
      //创建注册登录退出套接字
      _tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_tcpSocket < 0){
        perror("create tcpsocket error");
        exit(1);
      }
      //绑定地址信息
      sockaddr_in tcp_addr;
      tcp_addr.sin_family = AF_INET;
      tcp_addr.sin_port = htons(TCP_PORT);
      tcp_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
      ret = bind(_tcpSocket, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr));
      if(ret < 0){
        perror("udp bind error");
        exit(2);
      }
      ret = listen(_tcpSocket, 5);
      if(ret < 0){
        perror("listen falied");
        exit(3);
      }
      //创建数据池
      _messagePool = new msgPool;
      if(_messagePool == NULL){
        perror("_messagePool create error");
        exit(3);
      }
      //创建用户管理
      _userMng = new userMaganer();
      if(_userMng == NULL){
        perror("userMaganer create failed");
        exit(4);
      }
    }
    //创建线程
    void start(){
      //创建收发数据线程
      for(int i = 0; i < THREAD_NUM; ++i){
        int ret = pthread_create(&th, NULL, pushStart, (void*)this);
        if(ret < 0){
          perror("thread create error");
        }
        ret = pthread_create(&th, NULL, popStart, (void*)this);
      }
      //创建请求处理线程, 调用accept获取连接
      while(1){
        sockaddr_in addr;
        socklen_t len = sizeof(addr);

        int newTcpSockfd = accept(_tcpSocket, (struct sockaddr*)&addr, &len);
        printf("tcp soket success\n");
        if(newTcpSockfd < 0){
          perror("tcp accept failed");
          continue;
        }
        connetInfo* conInfo = new connetInfo(newTcpSockfd, (void*)this);
        int ret = pthread_create(&th, NULL, requestStart, (void*)conInfo);
        if(ret < 0){
          perror("tcp thread create failed");
          continue;
        }
        printf("requestStart thread create success %d\n", __LINE__);
      }
    }
  private:
    
    //处理请求入口函数
    static void* requestStart(void* arg){
      connetInfo* conInfo = (connetInfo*)arg;
      int tcpSock = conInfo->getTcpSock();
      Server* server = (Server*)conInfo->getServer();
      char flag;
      printf("requestStart 进入线程接收标志位\n");
      int ret = recv(tcpSock, &flag, 1, 0);
      if(ret < 0){
        perror("recv failed");
        return NULL;
      }
      else if(ret == 0){
        perror("peer closed");
        return NULL;
      }
      uint64_t userId = -1;
      int userStat = -1;

      printf("收到了标志位:%c\n", flag);
      int ch = flag - '0';
      std::cout << "转换为int" << ch << std::endl;
      switch(ch){
        case LOGIN:
          userStat = server->login(tcpSock);
          break;
        case LOGOUT:
          userStat = server->logout(tcpSock);
          break;
        case REGESTER:
          userStat = server->Register(tcpSock, userId);
          break;
        default:
          break;
      }
      return NULL;
    }

    //处理登录请求
    int login(int tcpSock){
      loginRequest lg;
      int ret = recv(tcpSock, &lg, sizeof(lg), 0);
      if(ret < 0){
        perror("login recv failed");
        return -1;
      }
      replyRequest rr;
      rr._stat = _userMng->userLogin(lg); 
      rr._userId = lg._userId;
      //应答
      ret = send(tcpSock, &rr, sizeof(rr), 0);
      if(ret < 0){
        perror("replyRequest error");
        return -1;
      }
      return LOGIN;
    }

    //处理退出请求
    int logout(int tcpSock){
      logoutRequest lr;
      int ret = recv(tcpSock, &lr, sizeof(lr), 0);
      if(ret < 0){
        perror("logout recv error");
        return -1;
      }
      replyRequest rr;

      rr._stat = _userMng->userLogout(lr);
      //应答
      rr._userId = lr._userId;
      ret = send(tcpSock, &rr, sizeof(rr), 0);
      return LOGOUT;
    }

    //处理注册请求 /userId为出参
    int Register(int tcpSock, uint64_t& userId){
      printf("正在处理登录请求\n");
      registerRequest rr;
      int ret = recv(tcpSock, &rr, sizeof(rr), 0);
      if(ret < 0){
        perror("Register recv error");
        return -1;
      }
      replyRequest rp;
      rp._stat = _userMng->userRegister(rr, userId);

      //应答
      printf("处理登录请求成功, 应答成功\n");
      rp._userId = userId;
      ret = send(tcpSock, &rp, sizeof(rp), 0);
      if(ret < 0){
        perror("Register reply error");
      }
      return LOGOUT;
    }

    //接收数据入口函数
    static void* pushStart(void* arg){
      Server* ser = (Server*)arg;
      pthread_detach(pthread_self());
      while(1){
        //一直收数据
        ser->recvMessageFromClient();
      }
      return NULL;
    }

    //发送数据入口函数
    static void* popStart(void* arg){
      Server* ser = (Server*)arg;
      pthread_detach(pthread_self());
      while(1){
        //一直发数据
        ser->sendMessageToAll();
      }
    }
  private:

    //接收数据
    void recvMessageFromClient(){
      sockaddr_in dest;
      socklen_t len = sizeof(dest);
      messageInfo mesg;
      int ret = recvfrom(_sockfd, mesg._data, sizeof(mesg), 0, (struct sockaddr*)&dest, &len);
      if(ret < 0){
        perror("sendto : recvMessageFromClient error");
      }
      else{
        uint64_t userId = mesg._userId;
        //存放数据
        printf("信息及结构体 %s\n", mesg._data); 
        std::string data;
        data.assign(mesg._data, MESSAGE_SIZE);
        _messagePool->pushMsg(data);
        _userMng->addAddrPort(userId, dest, len);
        printf("存放数据: %s 客户id: %ld\n",data.c_str(), userId);
      }
    }

    //给所有客户端发送数据
    void sendMessageToAll(){
      //获取数据
      std::string data;
      _messagePool->popMsg(data);
      int num = 0;
      std::vector<userInfo> useVec = _userMng->getUserOnline(num);
      printf("拿到了数据: %s, 在线用户: %d 个\n", data.c_str(), num);
      for(int i = 0; i < num; ++i){
        sockaddr_in addr  = useVec[i]._addr;
        socklen_t len = useVec[i]._len;
        sendMessageToOne(data, addr, len);
      }
    }

    //给一个客户端发送数据
    void sendMessageToOne(std::string& data,sockaddr_in& destAddr, socklen_t destLen){
      //给一个客户端发送数据
      int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&destAddr, destLen);
      if(ret < 0){
        perror("sendto : sendMessageToOne error");
      }
      printf("sendTOone success\n");
    }
  private:
    //收发数据socket
    int _sockfd;
    //注册登录退出socket
    int _tcpSocket;
    pthread_t th;
    //数据池
    msgPool* _messagePool;
    //用户管理
    userMaganer* _userMng;
};
