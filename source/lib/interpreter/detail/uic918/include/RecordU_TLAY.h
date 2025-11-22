#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace interpreter::detail::uic
{
  class RecordU_TLAY : public AbstractRecord
  {
  public:
    RecordU_TLAY(::utility::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual common::Context interpret(common::Context &&context) override;
  };
}
