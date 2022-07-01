
#include "../include/RecordInterpreterU_HEAD.h"
#include "../include/Utility.h"

RecordInterpreterU_HEAD::RecordInterpreterU_HEAD(RecordHeader &&h) : header(std::move(h))
{
  header.ensure("U_HEAD", {"01"});
}

Context &RecordInterpreterU_HEAD::interpret(Context &context)
{
  context.addField("U_HEAD.companyCode", Utility::getAlphanumeric(context.getPosition(), 4));
  context.addField("U_HEAD.uniqueTicketKey", Utility::getAlphanumeric(context.getPosition(), 20));
  context.addField("U_HEAD.editionTime", Utility::getDateTime12(context.getPosition()));
  context.addField("U_HEAD.flags", Utility::getAlphanumeric(context.getPosition(), 1));
  context.addField("U_HEAD.editionLanguageOfTicket", Utility::getAlphanumeric(context.getPosition(), 2));
  context.addField("U_HEAD.secondLanguageOfContract", Utility::getAlphanumeric(context.getPosition(), 2));

  return context;
}
