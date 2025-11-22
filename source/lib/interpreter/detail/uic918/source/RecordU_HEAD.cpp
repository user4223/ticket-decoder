
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

  common::Context RecordU_HEAD::interpret(common::Context &&context)
  {
    auto recordJson = ::utility::JsonBuilder::object();
    recordJson
        .add("companyCode", common::getAlphanumeric(context.getPosition(), 4))
        .add("uniqueTicketKey", common::getAlphanumeric(context.getPosition(), 20))
        .add("editionTime", common::getDateTime12(context.getPosition()))
        .add("flags", common::getAlphanumeric(context.getPosition(), 1))
        .add("editionLanguageOfTicket", common::getAlphanumeric(context.getPosition(), 2))
        .add("secondLanguageOfContract", common::getAlphanumeric(context.getPosition(), 2));

    context.addRecord(common::Record(header.recordId, header.recordVersion, std::move(recordJson)));
    return std::move(context);
  }
}
