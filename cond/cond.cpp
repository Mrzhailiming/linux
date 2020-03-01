#include <stdio.h>
#include <iostream>
#include <pthread.h>

#define THREADNUM 4
pthread_mutex_t mt;
pthread_cond_t proCond, conCond;//生产者消费者条件变量
int g_count = 0;
void* proStart(void* arg){
  pthread_cond_t* cond = (pthread_cond_t*)arg;
  int count = 0;
  while(1){
    pthread_mutex_lock(&mt);
    while(g_count == 1){
      pthread_cond_wait(&proCond, &mt);
    }
    g_count++;
    count++;
    printf("第 [%d] 次 produc [%p][%d]\n",count, (void*) pthread_self(), g_count);
    pthread_mutex_unlock(&mt);
    pthread_cond_signal(cond);
  }
  return NULL;
}
void* consumStart(void* arg){
  pthread_cond_t* cond = (pthread_cond_t*)arg;
  int count  = 0;
  while(1){
    pthread_mutex_lock(&mt);
    while(g_count == 0){
      pthread_cond_wait(&conCond, &mt);
    }
    g_count--;
    ++count;
    printf("第 [%d] 次 consum [%p][%d]\n",count, (void*) pthread_self(), g_count);
    pthread_mutex_unlock(&mt);
    pthread_cond_signal(cond);
  }
  return NULL;
}
int main(){
  pthread_t consum[THREADNUM];
  pthread_t produc[THREADNUM];
  //初始化锁, 条件变量
  pthread_mutex_init(&mt, NULL);
  pthread_cond_init(&proCond, NULL);
  pthread_cond_init(&proCond, NULL);

  int i = 0;
  for(; i < THREADNUM; ++i){
    int ret = pthread_create(&consum[i], NULL, consumStart, (void*)&proCond);
    if(ret != 0){
      perror("thread create error :");
      return 0;
    }
    ret = pthread_create(&produc[i], NULL, proStart, (void*)&conCond);
    if(ret != 0){
      perror("thread create error :");
      return 0;
    }
  }
  //线程等待
  for(i = 0; i < THREADNUM; ++i){
    pthread_join(consum[i], NULL);
    pthread_join(produc[i], NULL);
  }
  return 0;
}

