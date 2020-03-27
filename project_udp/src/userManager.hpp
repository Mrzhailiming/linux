#pragma once 
#include <arpa/inet.h>
#include <unordered_map>
#include <string.h>
#include <vector>
#include <algorithm>

#include <map>
#include "request.hpp"

//负责存储所有用户/在线用户信息
//提供用户注册登录退出接口
//单个用户信息
struct userInfo{
    uint64_t _userId;
    char _name[NAME_SIZE];
    char _school[SCHOOL_SIZE];
    char _password[PASSWORD_SIZE];
    sockaddr_in _addr;
    socklen_t _len;
    int _stat;
};

//管理所有用户,在线用户
class userMaganer{
  public:
  userMaganer(){
    pthread_mutex_init(&_mt, NULL);
    _id = new uint64_t();
  }
  ~userMaganer(){
    pthread_mutex_destroy(&_mt);
    delete _id;
  }
  //用户注册/ 不填充地址信息
  int userRegister(registerRequest& rg, uint64_t& userId){
    userInfo user;
    memcpy(user._name, rg._name, NAME_SIZE);
    memcpy(user._password, rg._password, PASSWORD_SIZE);
    memcpy(user._school, rg._school, SCHOOL_SIZE);
    user._stat = REGESTER;
    user._userId = getNewId();
    userId = user._userId;
    //保证原子操作
    pthread_mutex_lock(&_mt);
    _usersMap.insert(std::make_pair(user._userId, user));
    pthread_mutex_unlock(&_mt);
    return REGESTER;
  }

  //用户登录
  int userLogin(loginRequest& lg){
    uint64_t userId = lg._userId;
    std::map<uint64_t, userInfo>::iterator it = _usersMap.find(userId);
    //id不对
    if(it == _usersMap.end()){
      return -1;
    }
    //密码不对
    if(strcmp(lg._password, it->second._password) != 0){
      return -1;
    }
    _usersOnline.push_back(it->second);
    return LOGIN;
  }

  //用户退出
  int userLogout(logoutRequest& lr){
    uint64_t userId = lr._userId;
    //std::vector<userInfo>::iterator it = std::find(_usersOnline.begin(), _usersOnline.end(),_usersMap[userId]);
    //if(it == _usersOnline.end()){
    //  return -1;
    //}
    //_usersOnline.erase(it);
    for(auto& info : _usersOnline){
      if(info._userId == userId){
        info._stat = LOGOUT;
      }
    }
    return LOGOUT;
  }
  std::vector<userInfo>& getUserOnline(int& num){
    num = _usersOnline.size();
    return _usersOnline;
  }
  uint64_t getNewId(){
    return (*_id)++;
  }
  void addAddrPort(uint64_t userId, sockaddr_in addr, socklen_t len){
    std::map<uint64_t, userInfo>::iterator it = _usersMap.find(userId);
    if(it == _usersMap.end()){
      it->second._addr = addr;
      it->second._len = len;
      _usersOnline.push_back(it->second);
      printf("注册信息成功 %s %d\n", __FILE__, __LINE__);
      return;
    }
    printf("客户不存在!%s %d\n", __FILE__, __LINE__);
  }
  private:
    //存放所有用户信息
    uint64_t* _id;
    std::map<uint64_t, userInfo> _usersMap;
    pthread_mutex_t _mt;
    //存放在线用户信息
    std::vector<userInfo> _usersOnline;
};
