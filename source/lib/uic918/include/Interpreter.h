#pragma once

#include "Context.h"
#include "Field.h"

#include <string>
#include <map>

struct Interpreter
{
  virtual ~Interpreter() = default;

  virtual Context &interpret(Context &context) = 0;

  static std::map<std::string, Field> interpret(Context::BytesType const &input);
};
