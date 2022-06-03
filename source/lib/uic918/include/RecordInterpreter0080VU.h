#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

class RecordInterpreter0080VU : public Interpreter
{
  RecordHeader header;

public:
  RecordInterpreter0080VU(RecordHeader &&header);

  virtual Context &interpret(Context &context) override;
};
