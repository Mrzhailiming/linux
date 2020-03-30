#include <stdio.h>
#include <stdlib.h>
#include <istream>



#include "server.hpp"
#include "message.hpp"
#include "clientServer.hpp"


#define TCP_PORT 1997
#define UDP_PORT 1999



//注册
bool Register(clientServer& cs){
  std::string name;
  std::string school;
  std::string password;
  printf("请输入姓名:");
  fflush(stdout);
  std::cin >> name;
  printf("请输入学校:");
  fflush(stdout);
  std::cin >> school;
  printf("请输入密码:");
  fflush(stdout);
  std::cin >> password;

  //填充clientServer
  cs.sendRequest(REGESTER, 0, name, school, password);
  cs.getName() = name;
  cs.getSchool() = school;
  //接收应答
  replyRequest rp;
  cs.recvReply(rp);
  if(rp._stat == REGIST_SUCCESS){
    cs.getId() = rp._userId;
    printf("your id is %ld\n", rp._userId);
    return true;
  }
  else{
    return false;
  }
}

//登录
bool Login(clientServer& cs){
  uint64_t userId;
  std::string password;
  printf("请输入ID:");
  fflush(stdout);
  std::cin >> userId;
  printf("请输入密码:");
  fflush(stdout);
  std::cin >> password;
  cs.sendRequest(LOGIN, userId, "", "", password);

  LOG("INFO", "sendRequest success wait recvReply");
  replyRequest rp;
  cs.recvReply(rp);
  if(rp._stat == LOGIN_SUCCESS){
    cs.getStat() = ONLINE;
    cs.getId() = rp._userId;
    printf("LOGIN_SUCCESS %ld\n", cs.getId());
    return true;
  }
  printf("LOGIN_FAILED %d\n", rp._stat);
  return false;
}


//退出
bool Logout(clientServer& cs){
  return true;
}

bool menu(clientServer& clientSer){
  printf("**************************\n");
  printf("***1.登录 2.登出 3.注册***\n");
  printf("**************************\n");
  char ch;
  printf("请输入选项:");
  fflush(stdout);
  std::cin >> ch;
  switch(ch - '0'){
    case LOGIN:
      Login(clientSer);
      break;
    case LOGOUT:
      Logout(clientSer);
      break;
    case REGESTER:
      Register(clientSer);
      break;
    default:
       LOG("INFO", "选项不合法");
      return false;
      break;
  }
  if(clientSer.getStat() == OFFLINE){
    return false;
  }
  return true;
}

void* recvStart(void* arg){
  clientServer* cs = (clientServer*)arg;
  pthread_detach(pthread_self());
  while(1){
    std::string name;
    std::string school;
    std::string rec;
    cs->recvMesg(rec, name, school);
    std::cout << "[" << name << " "<< "sa ] : " << rec << "[from-school : " << school << " ]" << std::endl;
  }
  return NULL;
}

void* sendStart(void* arg){
  clientServer* cs = (clientServer*)arg;
  pthread_detach(pthread_self());
  while(1){
    std::cout << "[--my--] : " ;
    fflush(stdout);
    std::string data;
    std::cin >> data;
    cs->sendMesg(data);
  }
  return NULL;
}
int main(){
  clientServer cs;
  cs.initServer();
  while(menu(cs) == false){
    cs.resetTcpsock();
  }
  printf("---------登录成功---------\n");
  //创建线程收发数据
  pthread_t th;
  int ret = pthread_create(&th, NULL, recvStart, (void*)&cs);
  if(ret < 0){
    LOG("ERROR", "create recv thread failed");
    return 0;
  }
  ret = pthread_create(&th, NULL, sendStart, (void*)&cs);
  if(ret < 0){
    LOG("ERROR", "create recv thread failed");
    return 0;
  }
  while(1){
    sleep(100);
  }
  return 0;
}
