#pragma once 
#include <iostream>

#define  LOGIN 1
#define  LOGOUT 2
#define  REGESTER 3

#define NAME_SIZE 24
#define SCHOOL_SIZE 24
#define PASSWORD_SIZE 16
#define MESSAGE_SIZE 1024
//登录/注册/退出请求
struct loginRequest{
    uint64_t _userId;
    char _password[PASSWORD_SIZE];
};

struct logoutRequest{
    uint64_t _userId;
};

struct registerRequest{
    char _name[NAME_SIZE];
    char _school[SCHOOL_SIZE];
    char _password[PASSWORD_SIZE];
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


struct messageInfo{
  uint64_t _userId;
  char _data[MESSAGE_SIZE];
  int _capacity;
};
