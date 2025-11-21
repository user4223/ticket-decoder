
#include "../include/RecordU_HEAD.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

namespace interpreter::detail::uic
{
  RecordU_HEAD::RecordU_HEAD(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("U_HEAD", {"01"});
  }

  Context RecordU_HEAD::interpret(Context &&context)
  {
    auto recordJson = ::utility::JsonBuilder::object();
    recordJson
        .add("companyCode", getAlphanumeric(context.getPosition(), 4))
        .add("uniqueTicketKey", getAlphanumeric(context.getPosition(), 20))
        .add("editionTime", getDateTime12(context.getPosition()))
        .add("flags", getAlphanumeric(context.getPosition(), 1))
        .add("editionLanguageOfTicket", getAlphanumeric(context.getPosition(), 2))
        .add("secondLanguageOfContract", getAlphanumeric(context.getPosition(), 2));

    context.addRecord(Record(header.recordId, header.recordVersion, std::move(recordJson)));
    return std::move(context);
  }
}
