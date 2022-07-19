#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

class RecordInterpreterU_HEAD : public AbstractRecord
{
public:
  RecordInterpreterU_HEAD(RecordHeader &&header);

  virtual Context &interpret(Context &context) override;
};
