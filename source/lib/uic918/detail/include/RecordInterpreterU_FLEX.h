#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace uic918::detail
{
  class RecordInterpreterU_FLEX : public AbstractRecord
  {
  public:
    RecordInterpreterU_FLEX(RecordHeader &&header);

    virtual Context &interpret(Context &context) override;
  };
}
