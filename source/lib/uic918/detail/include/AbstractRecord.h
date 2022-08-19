#pragma once

#include "Interpreter.h"
#include "RecordHeader.h"

namespace uic918::detail
{

  class AbstractRecord : public Interpreter
  {
  protected:
    RecordHeader header;

    AbstractRecord(RecordHeader &&header);
  };

}
