#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include <string>

#include <pthread.h>
#include <vector>
#include "clientServer.hpp"

class chatWindow;

//需要包含 chatWindow clientServer以供线程入口函数调用 
class threadArg{
  public:
    threadArg(chatWindow* cwPtr, clientServer* csPtr, int num){
      _chatWindowPtr = cwPtr;
      _clientServerPtr = csPtr;
      _num = num;
    }
    chatWindow* _chatWindowPtr;
    clientServer* _clientServerPtr;
    int _num;
};


class chatWindow{
  public:
    chatWindow(){
      _head = NULL;
      _body = NULL;
      _right = NULL;
      _in = NULL;
      //清空vector
      _threadVec.clear();
      //初始化锁
      pthread_mutex_init(&_mt, NULL);
      //初始化窗口
      initscr();
      //光标不可见
      curs_set(0);
      //得到窗口的y x
      getmaxyx(stdscr, _maxLine, _maxCol);
    }
    ~chatWindow(){
      if(_head){
        delwin(_head);
      }
      if(_body){
        delwin(_body);
      }
      if(_right){
        delwin(_right);
      }
      if(_in){
        delwin(_in);
      }
      endwin();
      pthread_mutex_destroy(&_mt);
    }
    void makeHead(){
      int line = _maxLine / 5;
      int col = _maxCol;
      int y = 0;
      int x = 0;
      _head = newwin(line, col, y, x);
      box(_head, 0, 0);
      std::string data = "welcome to chat";
      //在哪个位置打印
        wmove(_head, line / 2, col / 2 - data.size() / 2);
        //wmove(_head, 1, 1);
        wprintw(_head, "%s", data.c_str());
        _wrefresh(_head);
    }

    void makeBody(clientServer* cs){
      //窗口的大小, 以及位置
      int line = _maxLine * 3 / 5;
      int col = _maxCol * 4 / 5;
      int y = _maxLine / 5;
      int x = 0;
      _body = newwin(line, col, y, x);
      //设置边框
      box(_body, '^', '^');

      wmove(_body, 1, 1);
      wprintw(_body, "%s", "look here");
      _wrefresh(_body);
      //
      int nowLine = 1;
      while(1){
        std::string data;
        std::string name;
        std::string school;
        cs->recvMesg(data, name, school);
        //移动光标
        wmove(_body, nowLine, 1);
        wprintw(_body, "[%s]%s [%s]",name.c_str(), data.c_str(), school.c_str());
        _wrefresh(_body);
        //清空窗口
        if(++nowLine == line - 1){
          wclear(_body);//刷新窗口, 边框也会被刷掉
          nowLine = 1;
          box(_body, '^', '^');
        }
      }
    }
    void makeRight(clientServer* cs){
      int line = _maxLine * 3 / 5;
      int col = _maxCol / 5;
      int y = _maxLine / 5;
      int x = _maxCol * 4 / 5;
      _right = newwin(line, col, y, x);
      box(_right, '*', '+');
      wmove(_right, 1, col / 2 - 4);
        wprintw(_right, "[%s]-[%s]","name", "school");
        _wrefresh(_right);
    }
    void makeIn(clientServer* cs){
      int line = _maxLine / 5;
      int col = _maxCol;
      int y = _maxLine * 4 / 5;
      int x = 0;
      _in = newwin(line, col, y, x);
      box(_in, '*', '+');
      curs_set(1);
      while(1){
        wmove(_in, 1, 1);
        wprintw(_in, "%s", "you say:");
        _wrefresh(_in);
        std::string data;
        char buf[1024 * 10] = {0};
        wscanw(_in, "%s", buf);
        int ret = strlen(buf);
        data.assign(buf, ret);
        cs->sendMesg(data);
        //每次输入后清空窗口
        wclear(_in);
        box(_in, '*', '+');
      }
    }
    static void* startFun(void* arg){

      threadArg* td = (threadArg*)arg;
      clientServer* clientServerPtr = td->_clientServerPtr;
      chatWindow* chatWindowPtr = td->_chatWindowPtr;
      int num = td->_num;
      switch(num){
        case 0:
          chatWindowPtr->makeHead();
          break;
        case 1:
          chatWindowPtr->makeBody(clientServerPtr);
          break;
        case 2:
          chatWindowPtr->makeRight(clientServerPtr);
          break;
        case 3:
          chatWindowPtr->makeIn(clientServerPtr);
          break;
        default:
          break;
      }
      while(1){
        sleep(10);
      }
      return NULL;
    }

    bool startWindow(clientServer* clientServerPtr){
      pthread_t th;
      //创建四个线程维护四个窗口
      for(int i = 0; i < 4; ++i){
        threadArg* arg = new threadArg(this, clientServerPtr, i);
        int ret = pthread_create(&th, NULL, startFun, (void*)arg);
        if(ret < 0){
          printf("create thread failed\n");
          exit(1);
        }
        _threadVec.push_back(th);
      }
      return true;
    }

    //wrefresh 线程不安全
    void _wrefresh(WINDOW* win){
      pthread_mutex_lock(&_mt);
      wrefresh(win);
      pthread_mutex_unlock(&_mt);
    }
  private:
    WINDOW* _head;
    WINDOW* _body;
    WINDOW* _right;
    WINDOW* _in;
    int _maxLine;
    int _maxCol;
    pthread_mutex_t _mt;

    std::vector<pthread_t> _threadVec;
};
