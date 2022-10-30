#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace uic918::detail
{
  class RecordU_TLAY : public AbstractRecord
  {
  public:
    RecordU_TLAY(::utility::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual Context &interpret(Context &context) override;
  };
}
