#include <ncurses.h>
#include <unistd.h>
#include <iostream>
#include <string>


int main(){
  int maxLine, maxCol;
  initscr();//初始化屏幕
  curs_set(0);//不显示光标
  getmaxyx(stdscr, maxLine, maxCol);
  int line = maxLine / 5;
  int col = maxCol;
  int y = 0;
  int x = 0;
  WINDOW* win = subwin(stdscr, line, col, y, x);
  box(win, '*', '*');//边框

  std::string data = "welcome to chat server";
  wmove(win, line / 2, col / 2 - data.size() / 2);
  wprintw(win, "%s", data.c_str());
  refresh();

  WINDOW* body = subwin(stdscr, line * 4, col, line, 0);
  box(body, '#', '#');//边框
  wmove(body, 1,1);
  char buf[1024] = {0};
  wscanw(body, "%s", buf);
  wprintw(body, "%s", buf);
  refresh();
  delwin(body);
 // 销毁
  delwin(win);
  while(1){
    sleep(10);
  }
  return 0;
}
