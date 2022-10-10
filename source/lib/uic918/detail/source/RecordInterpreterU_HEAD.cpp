
#include "../include/RecordInterpreterU_HEAD.h"
#include "../include/Utility.h"

namespace uic918::detail
{
  RecordInterpreterU_HEAD::RecordInterpreterU_HEAD(RecordHeader &&h)
      : AbstractRecord(std::move(h))
  {
    header.ensure("U_HEAD", {"01"});
  }

  Context &RecordInterpreterU_HEAD::interpret(Context &context)
  {
    context.addField("U_HEAD.companyCode", utility::getAlphanumeric(context.getPosition(), 4));
    context.addField("U_HEAD.uniqueTicketKey", utility::getAlphanumeric(context.getPosition(), 20));
    context.addField("U_HEAD.editionTime", utility::getDateTime12(context.getPosition()));
    context.addField("U_HEAD.flags", utility::getAlphanumeric(context.getPosition(), 1));
    context.addField("U_HEAD.editionLanguageOfTicket", utility::getAlphanumeric(context.getPosition(), 2));
    context.addField("U_HEAD.secondLanguageOfContract", utility::getAlphanumeric(context.getPosition(), 2));

    return context;
  }
}
