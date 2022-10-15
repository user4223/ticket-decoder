#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace uic918::detail
{
  class Record0080VU : public AbstractRecord
  {
  public:
    Record0080VU(RecordHeader &&header);

    virtual Context &interpret(Context &context) override;
  };
}
