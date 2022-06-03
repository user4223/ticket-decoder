#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

class RecordInterpreterU_HEAD : public Interpreter
{
  RecordHeader header;

public:
  RecordInterpreterU_HEAD(RecordHeader &&header);

  virtual Context &interpret(Context &context) override;
};
