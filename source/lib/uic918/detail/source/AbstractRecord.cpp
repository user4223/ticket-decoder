
#include "../include/AbstractRecord.h"

namespace uic918::detail
{
  AbstractRecord::AbstractRecord(::utility::Logger &&l, RecordHeader &&h)
      : logger(std::move(l)), header(std::move(h))
  {
  }
}
