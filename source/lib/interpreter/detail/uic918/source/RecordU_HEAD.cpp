
#include "../include/RecordU_HEAD.h"
#include "../include/Utility.h"

#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

namespace uic918::detail
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
        .add("companyCode", utility::getAlphanumeric(context.getPosition(), 4))
        .add("uniqueTicketKey", utility::getAlphanumeric(context.getPosition(), 20))
        .add("editionTime", utility::getDateTime12(context.getPosition()))
        .add("flags", utility::getAlphanumeric(context.getPosition(), 1))
        .add("editionLanguageOfTicket", utility::getAlphanumeric(context.getPosition(), 2))
        .add("secondLanguageOfContract", utility::getAlphanumeric(context.getPosition(), 2));

    context.addRecord(api::Record(header.recordId, header.recordVersion, std::move(recordJson)));
    return std::move(context);
  }
}
