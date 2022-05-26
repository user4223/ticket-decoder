#pragma once

#include "Interpreter.h"

class MainRecordInterpreter : public Interpreter
{
public:
  virtual Context interpret(Context &&context);
};
