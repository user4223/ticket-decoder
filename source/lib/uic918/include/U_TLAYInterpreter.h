#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

class U_TLAYInterpreter : public Interpreter
{
  RecordHeader header;

public:
  U_TLAYInterpreter(RecordHeader &&header);

  virtual Context &interpret(Context &context) override;
};
