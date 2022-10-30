#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace uic918::detail
{
  class RecordU_FLEX : public AbstractRecord
  {
  public:
    RecordU_FLEX(::utility::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual Context &interpret(Context &context) override;
  };
}
