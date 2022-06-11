
#include "../include/RecordInterpreterU_HEAD.h"
#include "../include/Utility.h"

#include <stdexcept>

RecordInterpreterU_HEAD::RecordInterpreterU_HEAD(RecordHeader &&h) : header(std::move(h)) {}

Context &RecordInterpreterU_HEAD::interpret(Context &context)
{
  if (header.recordId.compare("U_HEAD") != 0 || header.recordVersion.compare("01") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.toString());
  }

  context.addField("U_HEAD.recordId", header.recordId);
  context.addField("U_HEAD.recordVersion", header.recordVersion);
  context.addField("U_HEAD.recordLength", std::to_string(header.recordLength));

  context.addField("U_HEAD.companyCode", Utility::getAlphanumeric(context.getPosition(), 4));
  context.addField("U_HEAD.uniqueTicketKey", Utility::getAlphanumeric(context.getPosition(), 20));
  context.addField("U_HEAD.editionTime", Utility::getAlphanumeric(context.getPosition(), 12)); // DDMMYYYYHHMM
  context.addField("U_HEAD.flags", Utility::getAlphanumeric(context.getPosition(), 1));
  context.addField("U_HEAD.editionLanguageOfTicket", Utility::getAlphanumeric(context.getPosition(), 2));
  context.addField("U_HEAD.secondLanguageOfContract", Utility::getAlphanumeric(context.getPosition(), 2));

  return context;
}
