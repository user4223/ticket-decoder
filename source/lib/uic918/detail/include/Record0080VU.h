#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace uic918::detail
{
  class RecordInterpreter0080VU : public AbstractRecord
  {
  public:
    RecordInterpreter0080VU(RecordHeader &&header);

    virtual Context &interpret(Context &context) override;
  };
}
