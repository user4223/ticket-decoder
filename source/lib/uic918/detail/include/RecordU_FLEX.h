#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace uic918::detail
{
  class RecordU_FLEX : public AbstractRecord
  {
  public:
    RecordU_FLEX(RecordHeader &&header);

    virtual Context &interpret(Context &context) override;
  };
}
