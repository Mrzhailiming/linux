#pragma once 
#include <iostream>

//什么样的请求请求
#define  LOGIN 1 //登录请求
#define  LOGOUT 2 //退出请求
#define  REGESTER 3 //注册请求

//数据大小
#define NAME_SIZE 20    //名字长度
#define SCHOOL_SIZE 20  //学校长度
#define PASSWAOR_SIZE 16//密码长度

//处理请求的结果
enum dealResult{
  REGIST_SUCCESS = 4,//注册成功
  LOGIN_SUCCESS,     //登录成功
  REGIST_FAILED,     //注册失败
  LOGIN_FAILED,      //登录失败
  LOGOUT_SUCCESS,    //退出成功
  LOGOUT_FAILED      //退出失败
};


//用户的状态
enum userStat{
  ONLINE = 10, //在线
  OFFLINE,     //不在线
  LOGINED,     //已经注册

};
//登录
struct loginRequest{
    uint64_t _userId;
    char _password[PASSWAOR_SIZE];
};

//退出
struct logoutRequest{
    uint64_t _userId;
};

//注册
struct registerRequest{
    char _name[NAME_SIZE];
    char _school[SCHOOL_SIZE];
    char _password[PASSWAOR_SIZE];
};

//应答信息
struct replyRequest{
  uint64_t _userId;
  int _stat;
};



//提供tcp句柄＋server指针
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


