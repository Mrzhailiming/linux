#pragma once
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


#include "msgPool.hpp"
#include "request.hpp"
#include "userManager.hpp"
#include "log.hpp"
#include "message.hpp"

#define UDP_PORT 1999
#define TCP_PORT 1997
#define THREAD_NUM 4

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
        LOG("ERROR","create udpsocket error");
        exit(1);
      }
      LOG("INFO","create udpsocket success");
      //绑定地址信息
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(UDP_PORT);
      addr.sin_addr.s_addr = inet_addr("0.0.0.0");
      int ret = bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        LOG("ERROR","udp bind error");
        exit(2);
      }
      LOG("INFO","udp bind success");
      //创建注册登录退出套接字
      _tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_tcpSocket < 0){
        LOG("ERROR","create tcpsocket error");
        exit(1);
      }
      //绑定地址信息
      sockaddr_in tcp_addr;
      tcp_addr.sin_family = AF_INET;
      tcp_addr.sin_port = htons(TCP_PORT);
      tcp_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
      ret = bind(_tcpSocket, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr));
      if(ret < 0){
        LOG("ERROR","udp bind error");
        exit(2);
      }
      ret = listen(_tcpSocket, 5);
      if(ret < 0){
        LOG("ERROR","listen falied");
        exit(3);
      }
      //创建数据池
      _messagePool = new msgPool;
      if(_messagePool == NULL){
        LOG("ERROR","_messagePool create error");
        exit(3);
      }
      //创建用户管理
      _userMng = new userMaganer();
      if(_userMng == NULL){
        LOG("ERROR","userMaganer create failed");
        exit(4);
      }
      LOG("INFO", "initServer success");
    }
    //创建线程
    void start(){
      //创建收发数据线程
      for(int i = 0; i < THREAD_NUM; ++i){
        int ret = pthread_create(&th, NULL, pushStart, (void*)this);
        if(ret < 0){
          LOG("ERROR","thread create error");
        }
        ret = pthread_create(&th, NULL, popStart, (void*)this);
        if(ret < 0){
          LOG("ERROR","thread create error");
        }
      }
      LOG("INFO", "sendto and recvfrom thread create success");
      //创建请求处理线程, 调用accept获取连接
      while(1){
        sockaddr_in addr;
        socklen_t len = sizeof(addr);

        int newTcpSockfd = accept(_tcpSocket, (struct sockaddr*)&addr, &len);
        LOG("INFO", "a tcp had accept");
        if(newTcpSockfd < 0){
          LOG("WARNING","tcp accept failed");
          continue;
        }
        connetInfo* conInfo = new connetInfo(newTcpSockfd, (void*)this);
        int ret = pthread_create(&th, NULL, requestStart, (void*)conInfo);
        if(ret < 0){
          LOG("WARNING","tcp thread create failed");
          continue;
        }
        LOG("INFO","requestStart thread create success");
      }
    }
  private:
    
    //处理请求入口函数
    static void* requestStart(void* arg){
      connetInfo* conInfo = (connetInfo*)arg;
      int tcpSock = conInfo->getTcpSock();
      Server* server = (Server*)conInfo->getServer();
      char flag;
      int ret = recv(tcpSock, &flag, 1, 0);
      if(ret < 0){
        LOG("INFO","recv failed");
        return NULL;
      }
      else if(ret == 0){
        LOG("INFO","peer closed");
        return NULL;
      }
      uint64_t userId = -1;
      int userStat = -1;//为请求的处理结果
      switch(flag - '0'){
        case LOGIN:
          userStat = server->login(tcpSock, userId);
          break;
        case LOGOUT:
          userStat = server->logout(tcpSock, userId);
          break;
        case REGESTER:
          userStat = server->Register(tcpSock, userId);
          break;
        default:
          break;
      }
      //应答
      replyRequest rr;
      rr._stat = userStat;
      rr._userId = userId;
      LOG("INFO", "reply success");
      ret = send(tcpSock, &rr, sizeof(rr), 0);
      if(ret < 0){
        LOG("ERROR", "reply failed");
      }
      //关闭tcpsockfd,释放资源
      close(tcpSock);
      delete conInfo;
      return NULL;
    }

    //处理注册请求 /userId为出参, 返回值为是否注册成功
    int Register(int tcpSock, uint64_t& userId){
      registerRequest reg;
      int ret = recv(tcpSock, &reg, sizeof(reg), 0);
      if(ret < 0){
        LOG("ERROR","Register recv error");
        return REGIST_FAILED;
      }
      LOG("INFO", "dealwith Register ing");
      return _userMng->userRegister(reg, userId);
    }

    //处理登录请求, 返回值为是否登录成功
    int login(int tcpSock, uint64_t& userId){
      loginRequest lg;
      int ret = recv(tcpSock, &lg, sizeof(lg), 0);
      if(ret < 0){
        LOG("ERROR","login recv failed");
        return LOGIN_FAILED;
      }
      LOG("INFO", "dealwith login ing");
      userId = lg._userId;
      return _userMng->userLogin(lg);
    }

    //处理退出请求, 返回值为是否退出成功
    int logout(int tcpSock, uint64_t& userId){
      logoutRequest lr;
      int ret = recv(tcpSock, &lr, sizeof(lr), 0);
      if(ret < 0){
        LOG("ERROR","logout recv error");
        return LOGOUT_FAILED;
      }
      userId = lr._userId;
      return _userMng->userLogout(lr);
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
      char buf[1024 * 10] = {0};
      int ret = recvfrom(_sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&dest, &len);
      if(ret < 0){
        LOG("WARNING","recvMessageFromClient error");
      }
      else{
        //存放数据, 使用json反序列化mesg, 得到ID等用户信息
        std::string mesg;
        mesg.assign(buf, ret);
        //反序列化字符串
        Message json;
        json.Deser(mesg);//先获取data 和 userId

        //在addAddrPort中判断,如果用户第一次发消息,则填充地址信息
        int addStat = _userMng->addAddrPort(json.getUserId(), dest, len);
        if(addStat == -1){//用户不存在
          LOG("ERROR", "user not found");
        }
        else if(addStat == ONLINE){//用户是在线状态
          LOG("INFO", "user ONLINE dont need to add to OnlineUser");
        }
        //用户第一次发送消息, 需要从map中找到该用户并填充信息
        else{
          //存json串, 如果用户第一次发消息,需要填充json串里的name 和 school,
          //因为用户是直接登录的,并没有输入这些信息, 需要给json串填充name school
          //只需填充一次, 当客户端收到服务端群发的消息时,会填充上自己的name school,不会再发name为空的json串
          userInfo uInfo = _userMng->getUserInfo(json.getUserId());
          json.getName() = uInfo.getName();
          json.getSchool() = uInfo.getSchool();
          json.ser(mesg);//此时的mesg已经被填充上name school
        }
        //此时的mesg已经具有data, name, school
        _messagePool->pushMsg(mesg);
          printf("接收到一条数据->>> user: %ld, %s %s\n", json.getUserId(), json.getName().c_str(), json.getSchool().c_str());
      }
    }

    //给所有客户端发送数据
    void sendMessageToAll(){
      //获取数据
      std::string data;
      _messagePool->popMsg(data);
      std::vector<userInfo> userVec = _userMng->getOnlineUser();
      int count = 1;
      printf("total user->>>> %d\n", _userMng->getUserNum());
      for(auto& user : userVec){
        //群发消息
        //序列化
        //Message json;
        //json.getName() = user.getName();
        //json.getSchool() = user.getSchool();
        //json.getData() = data;
        //json.getUserId() = user.getId();
        
        //std::string out;
        //json.ser(out);
        //printf("---->>>>send no.%d userId: %ld, %s %s\n", count++, json.getUserId(), json.getName().c_str(), json.getSchool().c_str());
        printf("给第 %d 个用户发送消息\n", count++);
        sendMessageToOne(data, user.getAddr(), user.getLen());
      }
    }

    //给一个客户端发送数据
    void sendMessageToOne(std::string& data, struct sockaddr_in& destAddr, socklen_t destLen){
      //给一个客户端发送数据
      int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&destAddr, destLen);
      if(ret < 0){
        LOG("WARNING","sendMessageToOne failed");
        //缓存没发送的信息, 和用户
        return;
      }
      LOG("INFO", "sendTOone succes");
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
