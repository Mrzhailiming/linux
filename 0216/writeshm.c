#include <stdio.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY 0x12345678

int main(){
  int shmId = shmget(KEY, 1024, IPC_CREAT | 0664);
  if(shmId  < 0){
    perror("shmget error");
    return 0;
  }
  void* shmIp = shmat(shmId, NULL, 0);
  if(!shmIp){
    perror("write error ");
    return 0;
  }
  int i = 0;
  while(1){
    ++i;
    printf("write ok\n");
    sprintf((char*)shmIp, "%s -- %d\n", "write success", i);
    sleep(1);
  }  
  return 0;
}
