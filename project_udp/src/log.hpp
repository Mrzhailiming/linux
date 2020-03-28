#pragma once 
#include <iostream>
#include <string>
#include <stdlib.h>
inline void log(const std::string& str, const char* file, int line, const std::string& msg){
  std::cout << "log:" << str<< " " << file << " " << line << " " << msg << std::endl;
}

#define LOG(str, msg) log(str, __FILE__, __LINE__, msg)
