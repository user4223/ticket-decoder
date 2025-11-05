
#include "../include/Record118199.h"
#include "../include/Utility.h"

#include "lib/interpreter/api/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

namespace uic918::detail
{

  Record118199::Record118199(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("118199", {"01"});
  }

  Context Record118199::interpret(Context &&context)
  {
    auto const jsonString = utility::getAlphanumeric(context.getPosition(), context.getRemainingSize());

    context.addRecord(api::Record(header.recordId, header.recordVersion, ::utility::JsonBuilder(jsonString)));
    return std::move(context);
  }
}
