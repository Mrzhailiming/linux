#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX 1024
char com[MAX] = {0};



int menu(){
  memset(com, 0, MAX);
  printf("myshell$ ");
  fflush(stdout);
  if(scanf("%s", com) == 0){
    getchar();
    return -1;
  }
  return 0;
}

char** dealcom(char* com){
  int i = 0;
  static char* argv[32] = {NULL};
  char* ptr = com;
  while(*ptr != '\0'){
    if(*ptr != ' '){
      argv[i++] = ptr;
      while(*ptr != '\0' && *ptr != ' '){
        ++ptr;
      }
      continue;
    }
    *ptr = '\0';
    ++ptr;
  }
  argv[i] = NULL;
  return argv;
}

int do_exec(char* com){
  char** argv = {NULL};
  int ret = fork();
  if(ret == 0){
    argv = dealcom(com);
    if(argv[0] == NULL){
      exit(-1);
    }
    execvp(argv[0], argv);
  }
  else{
    waitpid(ret, NULL, 0);
  }
  return 0;
}

int main(int argc, char* argv[]){
  while(1){
    if(menu() < 0){
      printf("in error\n");
      continue;
    }
    do_exec(com);
  }
  return 0;
}
