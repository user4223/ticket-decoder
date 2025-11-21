
#include "../include/Record118199.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

namespace interpreter::detail::uic
{

  Record118199::Record118199(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("118199", {"01"});
  }

  Context Record118199::interpret(Context &&context)
  {
    auto const jsonString = getAlphanumeric(context.getPosition(), context.getRemainingSize());

    context.addRecord(Record(header.recordId, header.recordVersion, ::utility::JsonBuilder(jsonString)));
    return std::move(context);
  }
}
