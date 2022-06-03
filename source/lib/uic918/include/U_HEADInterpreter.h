#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

class U_HEADInterpreter : public Interpreter
{
  RecordHeader header;

public:
  U_HEADInterpreter(RecordHeader &&header);

  virtual Context &interpret(Context &context) override;
};
