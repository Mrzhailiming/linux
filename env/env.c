#include <stdio.h>
#include <stdlib.h>


int main(){
  extern char** environ;
  int i = 0;
  for(; environ[i]; ++i){
    //printf("%s\n", environ[i]);
  }
  char* env = getenv("MYENV");
  if(env){
    printf("%s\n", env);
  }
  return 0;
}
