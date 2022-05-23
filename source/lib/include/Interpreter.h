#pragma once

#include <map>
#include <string>

class Interpreter
{
public:
  struct Context
  {
    std::map<std::string, std::string> data;
  };

  virtual void interpret(Context &context) = 0;
};