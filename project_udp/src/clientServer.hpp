#pragma once 
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>

#include "log.hpp"
#include "message.hpp"
#include "request.hpp"

#define TCP_PORT 1997
#define UDP_PORT 1999
#define IP "192.168.88.128"

class clientServer{
  public:
    clientServer(){
      _tcpSock = -1;
      _udpSock = -1;
      _tcpPort = TCP_PORT;
      _udpPort = UDP_PORT;

      _userId = -1;
      _stat = OFFLINE;
    }
    ~clientServer(){
      if(_tcpSock > 0){
        close(_tcpSock);
      }
      if(_udpSock > 0){
        close(_udpSock);
      }
    }

    ///初始化
    bool initServer(){
      //创建tcp.udp套接字
      _tcpSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_tcpSock < 0){
        LOG("ERROR", "_tcpSock create failed");
        return false;
      }
      LOG("INFO", "_tcpSock create success");
      _udpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if(_tcpSock < 0){
        LOG("ERROR", "_udpSock create failed");
        return false;
      }
      LOG("INFO", "_udpSock create success");

      //连接
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(TCP_PORT);
      addr.sin_addr.s_addr = inet_addr(IP);
      int ret = connect(_tcpSock, (struct sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        LOG("ERROR", "connect failed");
        return false;
      }
      LOG("INFO", "connect success");
      return true;
    }

    bool sendMesg(std::string& data){
      //序列化
      Message msg;
      msg.setData(data);
      msg.setId(_userId);
      msg.setName(_name);
      msg.setSchool(_school);
      std::string out;
      msg.ser(out);

      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(UDP_PORT);
      addr.sin_addr.s_addr = inet_addr(IP);
      int ret = sendto(_udpSock, out.c_str(), out.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
      if(ret < 0){
        LOG("ERROR", "udp sendto failed");
        return false;
      }
      LOG("INFO", "udp sendto success");
      return true;
    }
    bool recvReply(replyRequest& rp){
      int ret = recv(_tcpSock, &rp, sizeof(rp), 0);
      if(ret < 0){
        LOG("ERROR", "recvReply failed");
        return false;
      }
      else if(ret == 0){
        LOG("ERROR", "server close tcp sock");
        return false;
      }
      LOG("INFO", "recvReply success");
      return true;
    }
    bool recvMesg(std::string& data, std::string& name, std::string& school){
      sockaddr_in addr;
      socklen_t len = sizeof(addr);
      char buf[BUFSIZ] = {0};
      int ret = recvfrom(_udpSock, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&addr, &len);
      if(ret < 0){
        LOG("ERROR", "udp recvMesg failed");
        return false;
      }
      //反序列化
      std::string msg;
      msg.assign(buf, ret);
      Message m;
      m.Deser(msg);
      data = m.getData();
      name = m.getName();
      school = m.getSchool();
      LOG("INFO", "udp recvMesg success");
      return true;
    }

    //发送请求
    bool sendRequest(char ch, uint64_t userId = -1, std::string name = "", std::string school = "", std::string password = ""){

      //先发送标志
      sendFlag(ch);
      //在发送请求信息
      switch(ch - '0'){
        case LOGIN:
          loginRequest li;
          li._userId = _userId;
          memcpy(li._password,password.c_str(), password.size());
          return _loginReq(li);
          break;
        case LOGOUT:
          logoutRequest lo;
          lo._userId = userId;
          return _logoutReq(lo);
          break;
        case REGESTER:
          registerRequest rg;
          memcpy(rg._name, name.c_str(), name.size());
          memcpy(rg._school, school.c_str(), school.size());
          memcpy(rg._password, password.c_str(), password.size());
          return _registReq(rg);
        default:
          LOG("INFO", "choice not correct");
          return false;
          break;
      }
      return false;;
    }

    //设置name school id 
    std::string& getName(){
      return _name;
    }
    std::string& getSchool(){
      return _name;
    }
    uint64_t& getId(){
      return _userId;
    }

    int getStat(){
      return _stat;
    }
  private:
    //发送标志
    bool sendFlag(char ch){
      int ret = send(_tcpSock, &ch, 1, 0);
      if(ret < 0){
        LOG("ERROR", "send flad failed");
        return false;
      }
      LOG("INFO", "send flad success");
      return true;
    }
    //发送登录请求
    bool _loginReq(loginRequest& li){
      int ret = send(_tcpSock, &li, sizeof(li), 0);
      if(ret < 0){
        LOG("ERROR", "send loginRequest failed");
        return false;
      }
      LOG("INFO", "send loginRequest success");
      return true;
    }
    //发送退出请求
    bool _logoutReq(logoutRequest& lo){
      int ret =send(_tcpSock, &lo, sizeof(lo), 0);
      if(ret < 0){
        LOG("ERROR", "send logoutRequest failed");
        return false;
      }
      LOG("INFO", "send logoutRequest success");
      return true;
    }
    //发送注册请求
    bool _registReq(registerRequest& rg){
      int ret =send(_tcpSock, &rg, sizeof(rg), 0);
      if(ret < 0){
        LOG("ERROR", "send registerRequest failed");
        return false;
      }
      LOG("INFO", "send registerRequest success");
      return true;
    }
    //设置用户id
    void setId(uint64_t userId){
      _userId = userId;
    }
  private:
    int _tcpSock;
    int _udpSock;
    uint16_t _tcpPort;
    uint16_t _udpPort;

    //用户信息
    uint64_t _userId;
    std::string _name;
    std::string _school;
    int _stat;
};
