#pragma once 
#include <iostream>
#include <queue>
#include <string>
#include <pthread.h>

#define QUEUE_SIZE 1024

//提供消息存放 和 获取
class msgPool{
  public:
    msgPool(){
      _queueSize = QUEUE_SIZE;
      pthread_cond_init(&_proCond, NULL);
      pthread_cond_init(&_conCond, NULL);
      pthread_mutex_init(&_mt, NULL);
    }

    //存放信息
    void pushMsg(std::string& msg){
      pthread_mutex_lock(&_mt);
      while(isFull()){
        pthread_cond_wait(&_proCond, &_mt);
      }
      _msgQueue.push(msg);
      pthread_mutex_unlock(&_mt);
      pthread_cond_signal(&_conCond);
    }
    //获取信息
    void popMsg(std::string& msg){
      pthread_mutex_lock(&_mt);
      while(empty()){
        pthread_cond_wait(&_conCond, &_mt);
      }
      msg = _msgQueue.front();
      _msgQueue.pop();
      pthread_mutex_unlock(&_mt);
      pthread_cond_signal(&_proCond);
    }
    bool isFull(){
      if(_msgQueue.size() == _queueSize){
        return true;
      }
      else{
        return false;
      }
    }
    bool empty(){
      return _msgQueue.size() == 0;
    }
  private:
    //队列
    std::queue<std::string> _msgQueue;
    size_t _queueSize;
    //条件变量
    pthread_cond_t _proCond;
    pthread_cond_t _conCond;
    //互斥锁
    pthread_mutex_t _mt;
};
