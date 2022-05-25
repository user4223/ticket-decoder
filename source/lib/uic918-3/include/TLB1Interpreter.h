#pragma once

#include "Interpreter.h"

class TLB1Interpreter : public Interpreter
{
public:
  virtual Context interpret(Context &&context);
};
