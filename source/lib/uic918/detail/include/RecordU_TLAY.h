#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace uic918::detail
{
  class RecordInterpreterU_TLAY : public AbstractRecord
  {
  public:
    RecordInterpreterU_TLAY(RecordHeader &&header);

    virtual Context &interpret(Context &context) override;
  };
}
