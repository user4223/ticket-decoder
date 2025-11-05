#pragma once

#include "lib/utility/include/LoggingFwd.h"

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "RecordHeader.h"

namespace uic918::detail
{

  class AbstractRecord : public Interpreter
  {
  protected:
    RecordHeader header;
    ::utility::Logger logger;

    AbstractRecord(::utility::Logger &&logger, RecordHeader &&header);
  };

}
