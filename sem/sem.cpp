#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
using namespace std;

#define THNUM 4
#define COUNT 10
template <class T>
class Model{
  public:
    Model()
    :_v(COUNT)
    ,_wpos(0)
    ,_rpos(0){
      //初始化充当锁的信号量为1
      sem_init(&_mu_sem, 0, 1);
      //生产者
      sem_init(&_pro_sem, 0, COUNT);
      //消费者初始资源为0
      sem_init(&_con_sem, 0, 0);
    }
    //消费者
    void Pop(T* out){
      //先判断有无资源
      sem_wait(&_con_sem);
      //再加锁
      sem_wait(&_mu_sem);
      *out = _v[_rpos];
      _rpos = (_rpos + 1) % COUNT;
      sem_post(&_pro_sem);//给生产者资源计数器+1
      sem_post(&_mu_sem);
    }
    //生产者
    void Push(T& val){
      //先判断有无资源
      sem_wait(&_pro_sem);
      //再加锁
      sem_wait(&_mu_sem);
      _v[_wpos] = val;
      _wpos = (_wpos + 1) % COUNT;
      sem_post(&_con_sem);//给消费者资源计数器+1
      sem_post(&_mu_sem);
    }
  private:
    //三个信号量
    sem_t _mu_sem, _pro_sem, _con_sem;
    //一个环形队列
    vector<T> _v;
    //读写位置
    int _wpos, _rpos;
};

void* conStart(void* arg){
  Model<int>* m = (Model<int>*)arg;
  int out = 0;
  while(1){
    m->Pop(&out);
    printf("con [%p][%d]\n", pthread_self(), out);
  }
  return NULL;
}
void* proStart(void* arg){
  Model<int>* m = (Model<int>*)arg;
  int out = 0;
  while(1){
    m->Push(out);
    printf("pro [%p][%d]\n", pthread_self(), out);
    ++out;
  }
  return NULL;
}
int main(){
  pthread_t pro_th[THNUM], con_th[THNUM];
  Model<int> m;
  int i = 0;
  for(; i < THNUM; ++i){
    int ret = pthread_create(&pro_th[i], NULL, proStart, &m);
    if(ret != 0){
      perror("create error");
    }
    ret = pthread_create(&con_th[i], NULL, conStart, &m);
    if(ret != 0){
      perror("create error");
    }
  }
  i = 0;
  for(; i < THNUM; ++i){
    pthread_join(pro_th[i], NULL);
    pthread_join(con_th[i], NULL);
  }
  return 0;
}
