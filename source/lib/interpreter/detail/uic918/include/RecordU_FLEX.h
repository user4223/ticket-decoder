#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace interpreter::detail::uic
{
  class RecordU_FLEX : public AbstractRecord
  {
  public:
    RecordU_FLEX(::utility::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual Context interpret(Context &&context) override;
  };
}
