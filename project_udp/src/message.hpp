#pragma once 

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "/usr/include/include/json/json.h"

class Message{
  public:
    //序列化, 把json格式转换为字符串
    void ser(std::string& msg){
      Json::Value val;
      val["_name"] = _name;
      val["_school"] = _school;
      val["_data"] = _data;
      val["_userId"] = std::to_string(_userId);
      msg = val.toStyledString();
    }

    //反序列化
    void Deser(std::string msg){
      Json::Reader read;
      Json::Value val;
      read.parse(msg, val, false);
      _name = val["_name"].asString();
      _school = val["_school"].asString();
      _data = val["_data"].asString();
      _userId = atol(val["_userId"].asString().c_str());
    }
    std::string& getName(){
      return _name;
    }
    std::string& getSchool(){
      return _school;
    }
    std::string& getData(){
      return _data;
    }
    uint64_t& getUserId(){
      return _userId;
    }
    
    void setName(std::string& name){
      _name = name;
    }
    void setSchool(std::string& school){
      _school = school;
    }
    void setData(std::string& data){
      _data = data;
    }
    void setUserId(uint64_t id){
      _userId = id;
    }
  private:
    std::string _name;
    std::string _school;
    std::string _data;
    uint64_t _userId;
};
