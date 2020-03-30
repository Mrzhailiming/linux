#pragma once 
#include <iostream>
#include <string>
#include <stdlib.h>

#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
inline void log(const std::string& str, const char* file, int line, const std::string& msg){
  int fd = open("./log.txt", O_CREAT | O_WRONLY | O_APPEND, 0777);
  std::string tmp;
  tmp.append("log:").append(str).append(" ").append(file).append(std::to_string(line)).append(" ").append(msg).append("\n");

  write(fd, tmp.data(), tmp.size());
  //std::cout << "log:" << str<< " " << file << " " << line << " " << msg << std::endl;
}

#define LOG(str, msg) log(str, __FILE__, __LINE__, msg)
