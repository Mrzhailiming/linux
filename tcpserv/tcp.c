#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

class tcpServ{
  public:
    tcpServ(){
      _sockfd = -1;
    }
    //创建套接字
    //绑定地址信息端口
    //监听
    //连接
    //获取连接
    //发送
    //接收
  private:
    int _sockfd;
};
