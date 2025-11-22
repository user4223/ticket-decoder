#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace interpreter::detail::uic
{
  class Record0080VU : public AbstractRecord
  {
  public:
    Record0080VU(::utility::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual common::Context interpret(common::Context &&context) override;
  };
}
