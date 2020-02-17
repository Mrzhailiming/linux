#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>

#define KEY 0x12345678

int main(){
  //得到共享内存,返回值是共享内存的名字,
  int shmID = shmget(KEY, 1024, IPC_CREAT | 0664);
  if(shmID < 0){
    perror("get error");
  }
  //将共享内存与进程虚拟地址空间连接,并获取共享内存的首地址
  void* shmIP = shmat(shmID, NULL, 0);
  if(!shmIP){
    perror("shmat error");
  }
  printf("success\n");
  while(1){
    printf("read success %s\n",(char*)shmIP);
    sleep(1);
  }
  //将共享内存脱离
  shmdt(shmIP);
  //while(1){
  //  printf("shmdt succss\n");
  //}
  //删除共享内存
  shmctl(shmID, IPC_RMID, NULL);
  return 0;
}
