#pragma once

#include "Interpreter.h"

class U_HEADInterpreter : public Interpreter
{
public:
  virtual Context &interpret(Context &context);
};
