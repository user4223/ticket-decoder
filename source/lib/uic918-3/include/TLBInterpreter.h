#pragma once

#include "Interpreter.h"

class TLBInterpreter : public Interpreter
{
public:
  virtual Context interpret(Context &&context);
};
