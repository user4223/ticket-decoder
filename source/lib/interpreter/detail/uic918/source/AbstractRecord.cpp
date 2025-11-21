
#include "../include/AbstractRecord.h"

namespace interpreter::detail::uic
{
  AbstractRecord::AbstractRecord(::utility::Logger &&l, RecordHeader &&h)
      : logger(std::move(l)), header(std::move(h))
  {
  }
}
