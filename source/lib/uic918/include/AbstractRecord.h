#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

class AbstractRecord : public Interpreter
{
protected:
  RecordHeader header;

  AbstractRecord(RecordHeader &&header);
};
