#pragma once 
#include <arpa/inet.h>
#include <unordered_map>
#include <string.h>
#include <algorithm>
#include <vector>
#include <map>


#include "request.hpp"
#include "log.hpp"


//负责存储所有用户/在线用户信息
//提供用户注册登录退出接口


//单个用户信息
class userInfo{
  public:

    //填充name school password
    void addNameSchPwd(const std::string& name, const std::string& school, const std::string& password){
      _name = name;
      _school = school;
      _password = password;
    }

    //填充addr len 
    void addAddrLen(const sockaddr_in& addr, const socklen_t len){
      memcpy(&_addr, &addr, len);
      _len = len;
    }

    //get用户udpaddr
    struct sockaddr_in& getAddr(){
      return _addr;
    }
    socklen_t& getLen(){
      return _len;
    }
    //get状态
    int& getStat(){
      return _stat;
    }

    //get密码
    std::string& getPwd(){
      return _password;
    }

    //get用户ID
    uint64_t& getId(){
      return _userId;
    }
    std::string& getName(){
      return _name;
    }
    std::string& getSchool(){
      return _school;
    }
  private:
    uint64_t _userId;
    std::string _name;
    std::string _school;
    std::string _password;
    sockaddr_in _addr;
    socklen_t _len;
    int _stat;
};

//管理所有用户,在线用户
class userMaganer{
  public:
  userMaganer(){
    pthread_mutex_init(&_mt, NULL);
    _newId = new int();
  }
  ~userMaganer(){
    pthread_mutex_destroy(&_mt);
    delete _newId;
  }
  //用户注册/ 不填充地址信息,返回值为是否登录成功
  int userRegister(registerRequest& reg, uint64_t& userId){

    if(strlen(reg._name) == 0 || strlen(reg._password) == 0 || strlen(reg._school) == 0){
      LOG("INFO", "regist failed");//注册失败
      return REGIST_FAILED;
    }
    //填充姓名学校密码
    userInfo newUser;
    LOG("INFO", "fill name school password");
    newUser.getPwd() = reg._password;
    newUser.addNameSchPwd(reg._name, reg._school, reg._password);
    newUser.getStat() = LOGINED;//更改状态为已注册
    printf("name:%s school:%s password: %s id:%ld\n", newUser.getName().c_str(), newUser.getSchool().c_str(), newUser.getPwd().c_str(), newUser.getId());
    
    //操作临界资源_newId 和线程不安全的unordered_map
    pthread_mutex_lock(&_mt);
    //获取新的id
    userId = (*_newId)++;
    newUser.getId() = userId;

    printf("name:%s school:%s password: %s id:%ld\n", reg._name,reg._school, reg._password, newUser.getId());
    //放入用户池
    _usersMap.insert(std::make_pair(userId, newUser));
    pthread_mutex_unlock(&_mt);
    printf("Register deal success, ID: %ld status : %d FILE : %s LINE %d\n", userId, newUser.getStat(), __FILE__, __LINE__);
    return REGIST_SUCCESS;
  }

  //用户登录, 返回是否登录成功
  int userLogin(loginRequest& lg){
    uint64_t userId = lg._userId;
    //保证线程安全
    pthread_mutex_lock(&_mt);
    std::unordered_map<uint64_t, userInfo>::iterator it = _usersMap.find(userId);
    //没找到用户/密码不对
    printf("log:INFO useId:%ld passwor:%s\n", userId, lg._password);
    if(it == _usersMap.end() || strcmp(it->second.getPwd().c_str(), lg._password) != 0){
      LOG("INFO", "user not found or pwd not correct");
      pthread_mutex_unlock(&_mt);
      return LOGIN_FAILED;
    }
    //更改状态为ONLINE
    it->second.getStat() = LOGINED;
    _usersOnline.push_back(it->second);
    pthread_mutex_unlock(&_mt);
    LOG("INFO", "user login success");
    return LOGIN_SUCCESS;
  }

  //用户退出, 返回是否退出成功
  int userLogout(logoutRequest& lr){
    uint64_t userId = lr._userId;
    std::unordered_map<uint64_t, userInfo>::iterator it = _usersMap.find(userId);
    if(it != _usersMap.end()){
      //更改状态
      it->second.getStat() = OFFLINE;
      return LOGOUT_SUCCESS;
    }
    return LOGOUT_FAILED;
  }

  //填充用户地址信息,成功返回0,失败返回-1
  int addAddrPort(const uint64_t userId, const sockaddr_in& addr, socklen_t len){

    //保证线程安全
    pthread_mutex_lock(&_mt);
    std::unordered_map<uint64_t, userInfo>::iterator it = _usersMap.find(userId);
    //用户不存在
    if(it == _usersMap.end()){
      pthread_mutex_unlock(&_mt);
      LOG("WARNING", "user not found");
      return -1;
    }
    //用户如果在线,就不用填充地址信息,
    if(it->second.getStat() == ONLINE){
      return 0;
    }
    //用户为LOGINED,则填充地址信息,并把状态改为ONLINE
    it->second.addAddrLen(addr, len);
    it->second.getStat() = ONLINE;

    pthread_mutex_unlock(&_mt);
    return 0;
  }

  //增加在线用户
  void addOnlineUser(uint64_t userId){
    pthread_mutex_lock(&_mt);
    std::unordered_map<uint64_t, userInfo>::iterator it = _usersMap.find(userId);
    if(it == _usersMap.end()){
      LOG("ERROR", "user not found");
    }
    else{
      _usersOnline.push_back(it->second);
    }
    pthread_mutex_unlock(&_mt);
  }
  //get在线用户
  std::vector<userInfo>& getOnlineUser(){
    return _usersOnline;
  }
  private:
    //存放所有用户信息
    std::unordered_map<uint64_t, userInfo> _usersMap;
    pthread_mutex_t _mt;
    //存放在线用户信息
    std::vector<userInfo> _usersOnline;
    int* _newId;
};
