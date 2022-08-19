
#include "../include/AbstractRecord.h"

namespace uic918::detail
{
  AbstractRecord::AbstractRecord(RecordHeader &&h) : header(std::move(h))
  {
  }
}
