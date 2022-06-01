#pragma once

#include "Interpreter.h"
#include "BlockHeader.h"

class U_HEADInterpreter : public Interpreter
{
  BlockHeader header;

public:
  U_HEADInterpreter(BlockHeader &&header);

  virtual Context &interpret(Context &context);
};
