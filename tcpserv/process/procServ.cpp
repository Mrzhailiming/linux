#include "tcp.hpp"
#include <sys/wait.h>
#include <signal.h>



//子进程退出会返回SIGCHLD
//操作系统默认处理方式为忽略
//需要自定义函数对子进程进行等待
void call(int errno){
  (void)errno;
  while(1){
    waitpid(-1, NULL, WNOHANG);
  }
}

int main(){
  signal(SIGCHLD, call);
  std::string ip = "192.168.88.128";
  uint16_t port = 1999;
  tcpServ ser;
  //创建套接字
  if(!ser.CreateSocket()){
    return 0;
  }
  //绑定ip 端口
  if(!ser.BindIpPort(ip, port)){
    return 0;
  }
  //监听
  if(!ser.Listen(5)){
    return 0;
  }
  //获取连接
  while(1){
    tcpServ out;
    if(!ser.Accept(out)){
      continue;
    }
    int ret = fork();
    if(ret < 0){
      perror("fork error");
      return 0;
    }
    else if(ret == 0){
      //子进程
      while(1){
      std::string ret;
      out.Recv(ret);
      printf("client send: %s\n", ret.c_str());

      printf("you say :");
      fflush(stdout);
      std::cin >> ret;
      out.Send(ret);
      } 
    }
    else{
      //父进程
      //父进程关闭新创建的套接字
      out.Close();
    }
  }
  return 0;
}
