#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace interpreter::detail::uic
{
  class Record118199 : public AbstractRecord
  {
  public:
    Record118199(::utility::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual Context interpret(Context &&context) override;
  };
}
