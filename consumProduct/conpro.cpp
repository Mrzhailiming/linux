#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>



#define THREADNUM 4
#define COUNT 10
template <class T>
class Model{
  public:
    Model(){
      pthread_mutex_init(&mt, NULL);
      pthread_cond_init(&proCond, NULL);
      pthread_cond_init(&csmCond, NULL);
    }
    T Pop(T* out){
      //加锁
      pthread_mutex_lock(&mt);
      //循环判断
      while(q.size() == 0){//队列空, pop等待
        //调用wait 进入pop等待队列
        pthread_cond_wait(&csmCond, &mt);
      }
      *out = q.front();
      q.pop();
      pthread_mutex_unlock(&mt);
      //唤醒push队列
      pthread_cond_signal(&proCond);
    }
    T Push(T val){
      //加锁
      pthread_mutex_lock(&mt);
      //循环判断
      while(q.size() == COUNT){//队列满, push等待
        //调用wait 进入push等待队列
        pthread_cond_wait(&proCond, &mt);
      }
      q.push(val);
      pthread_mutex_unlock(&mt);
      //唤醒pop队列
      pthread_cond_signal(&csmCond);
    }
    pthread_mutex_t mt;//一个互斥锁
  private:
    //生产者消费者模型
    //1 + 2 + 3
    //一个队列, 2个, 3种关系
    std::queue<T> q;
    pthread_cond_t proCond, csmCond;//两个条件变量
};

void* csmStart(void* arg){
  Model<int>* pMd = (Model<int>*)arg;
  while(1){
    int out = 0;
    pMd->Pop(&out);
    printf("csm [%p][%d]\n", (void*)pthread_self(), out);
    sleep(1);
  }
}
void* proStart(void* arg){
  Model<int>* pMd = (Model<int>*)arg;
    int i = 0;
  while(1){
    pMd->Push(i);
    printf("pro [%p][%d]\n", (void*)pthread_self(), i);
    ++i;
  }
}

int main(){
  //生产者消费者模型
  Model<int> md;
  pthread_t csm[THREADNUM], pro[THREADNUM];
  int i = 0;
  for(;i < THREADNUM; ++i){
    int ret = pthread_create(&csm[i], NULL, csmStart, (void*)&md);
    if(ret != 0){
      perror("create error :");
      return 0;
    }
    ret = pthread_create(&pro[i], NULL, proStart, (void*)&md);
    if(ret != 0){
      perror("create error :");
      return 0;
    }
  }
  //线程等待
  for(i = 0; i < THREADNUM; ++i){
    pthread_join(csm[i], NULL);
    pthread_join(pro[i], NULL);
  }
  return 0;
}
