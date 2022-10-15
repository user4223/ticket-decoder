
#include "../include/RecordInterpreterU_HEAD.h"
#include "../include/Utility.h"

#include "../../api/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"

namespace uic918::detail
{
  RecordInterpreterU_HEAD::RecordInterpreterU_HEAD(RecordHeader &&h)
      : AbstractRecord(std::move(h))
  {
    header.ensure("U_HEAD", {"01"});
  }

  Context &RecordInterpreterU_HEAD::interpret(Context &context)
  {
    auto recordJson = ::utility::JsonBuilder::object()
                          .add("companyCode", utility::getAlphanumeric(context.getPosition(), 4))
                          .add("uniqueTicketKey", utility::getAlphanumeric(context.getPosition(), 20))
                          .add("editionTime", utility::getDateTime12(context.getPosition()))
                          .add("flags", utility::getAlphanumeric(context.getPosition(), 1))
                          .add("editionLanguageOfTicket", utility::getAlphanumeric(context.getPosition(), 2))
                          .add("secondLanguageOfContract", utility::getAlphanumeric(context.getPosition(), 2));

    return context.addRecord(api::Record(header.recordId, header.recordVersion, recordJson.build()));
  }
}
