#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace interpreter::detail::uic
{
  class RecordU_HEAD : public AbstractRecord
  {
  public:
    RecordU_HEAD(::utility::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual common::Context interpret(common::Context &&context) override;
  };
}
