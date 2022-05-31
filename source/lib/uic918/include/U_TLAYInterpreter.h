#pragma once

#include "Interpreter.h"

class U_TLAYInterpreter : public Interpreter
{
public:
  virtual Context &interpret(Context &context);
};
