#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace uic918::detail
{
  class RecordU_HEAD : public AbstractRecord
  {
  public:
    RecordU_HEAD(RecordHeader &&header);

    virtual Context &interpret(Context &context) override;
  };
}
