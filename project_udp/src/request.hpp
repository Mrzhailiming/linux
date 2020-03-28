#pragma once 
#include <iostream>

#define  LOGIN 1
#define  LOGOUT 2
#define  REGESTER 3

enum dealResult{
  REGIST_SUCCESS = 6,
  LOGIN_SUCCESS,
  REGIST_FAILED,
  LOGIN_FAILED,
  LOGOUT_SUCCESS,
  LOGOUT_FAILED
};

enum userStat{
  ONLINE = 18,
  OFFLINE,
};
//登录
struct loginRequest{
    uint64_t _userId;
    std::string _password;
};

//退出
struct logoutRequest{
    uint64_t _userId;
};

//注册
struct registerRequest{
    std::string _name;
    std::string _school;
    std::string _password;
};

//应答信息
struct replyRequest{
  uint64_t _userId;
  int _stat;
};



class connetInfo{
  public:
    connetInfo(int sockfd, void* server){
      _sockfd = sockfd;
      _server = server;
    }

    int getTcpSock(){
      return _sockfd;
    }
    void* getServer(){
      return _server;
    }
  private:
    int _sockfd;
    void* _server;
};


