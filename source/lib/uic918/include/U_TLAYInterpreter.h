#pragma once

#include "Interpreter.h"
#include "BlockHeader.h"

class U_TLAYInterpreter : public Interpreter
{
  BlockHeader header;

public:
  U_TLAYInterpreter(BlockHeader &&header);

  virtual Context &interpret(Context &context);
};
