#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

class RecordInterpreterU_TLAY : public Interpreter
{
  RecordHeader header;

public:
  RecordInterpreterU_TLAY(RecordHeader &&header);

  virtual Context &interpret(Context &context) override;
};
