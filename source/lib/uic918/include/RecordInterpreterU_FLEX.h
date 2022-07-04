#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

class RecordInterpreterU_FLEX : public Interpreter
{
  RecordHeader header;

public:
  RecordInterpreterU_FLEX(RecordHeader &&header);

  virtual Context &interpret(Context &context) override;
};
