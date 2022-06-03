#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

class RecordInterpreter0080BL : public Interpreter
{
  RecordHeader header;

public:
  RecordInterpreter0080BL(RecordHeader &&header);

  virtual Context &interpret(Context &context) override;
};
