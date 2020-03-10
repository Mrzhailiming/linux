#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>
using namespace std;


#define THREADNUM 4
#define CAPACITY 10

//供任务类传函数地址
void handler1(int val){
  cout << "han1 ->" << val << endl;
}
void handler2(int val){
  cout << "han2 ->" << val << endl;
}

//任务类
typedef void (*handler_t)(int);
class task{
  public:
    task(){
      _val = -1;
      _handler = NULL;
    }
    task(int val,handler_t handler){
      _val = val;
      _handler = handler;
    }
    void run(){
      _handler(_val);
    }
    int _val;
    handler_t _handler;
};

//线程池
class threadPool{
  public:
    threadPool()
      :_capacity(CAPACITY)
      ,flag(false){
      //初始化锁
      pthread_mutex_init(&_mt, NULL);
      pthread_cond_init(&_cond, NULL);
      int i = 0;
      //创建线程
      for(; i < THREADNUM; ++i){
        int ret = pthread_create(&_thread[i], NULL, startFun, (void*)this);
        if(ret != 0){
          cout << "create error " << endl;
        }
      }
     _totalThread =  _curThread = THREADNUM;
    }
    //入口函数
    static void* startFun(void* arg){
      threadPool* tdpool = (threadPool*)arg;
      while(1){
        pthread_mutex_lock(&(tdpool->_mt));
        while(tdpool->empty()){
          //判断是不是需要退出
          if(tdpool->flag){
            tdpool->exitThread();
            cout << "falg == true" << endl;
          }
          //
          pthread_cond_wait(&(tdpool->_cond), &(tdpool->_mt));
        }
        task* ptask = tdpool->_que.front();
        tdpool->_que.pop();
        ptask->run();
        pthread_mutex_unlock(&tdpool->_mt);
      }
      return NULL;
    }
    void push(task* t){
      pthread_mutex_lock(&_mt);
      _que.push(t);
      pthread_mutex_unlock(&_mt);
      pthread_cond_broadcast(&_cond);//唤醒等待队列
    }

    void threadJoin(){
      int i = 0;
      for(; i < THREADNUM; ++i){
        pthread_join(_thread[i], NULL);
        cout << "join" << endl;
      }
    }
    //清除线程
    void clearThread(){
      pthread_mutex_lock(&_mt);
      flag = true;
      pthread_mutex_unlock(&_mt);
      //唤醒所有线程, 直到所有线程退出
      while(_curThread > 0){
        pthread_cond_broadcast(&_cond);
      }
    }
    //线程退出
    void exitThread(){
      --_curThread;
      //注意解锁
      pthread_mutex_unlock(&_mt);
      pthread_exit(NULL);
    }
    bool empty(){
      return _que.empty();
    }
  private:
    int _capacity;
    bool flag;
    //锁
    pthread_mutex_t _mt;
    //等待队列
    pthread_cond_t _cond;
    //线程
    pthread_t _thread[THREADNUM];
    int _totalThread;
    int _curThread;
    //队列
    queue<task*> _que;
};



int main(){
  threadPool pool;
  sleep(1);
  task t1(1, handler1);
  task t2(2, handler2);
  pool.push(&t1);
  pool.push(&t2);
  
  sleep(8);
  pool.clearThread();
  pool.threadJoin();
  return 0;
}
