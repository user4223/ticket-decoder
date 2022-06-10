
#include "../include/RecordInterpreterU_HEAD.h"
#include "../include/Utility.h"

#include <stdexcept>

RecordInterpreterU_HEAD::RecordInterpreterU_HEAD(RecordHeader &&h) : header(std::move(h)) {}

Interpreter::Context &RecordInterpreterU_HEAD::interpret(Context &context)
{
  if (header.recordId.compare("U_HEAD") != 0 || header.recordVersion.compare("01") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.to_string());
  }

  context.output.insert(std::make_pair("U_HEAD.recordId", header.recordId));
  context.output.insert(std::make_pair("U_HEAD.recordVersion", header.recordVersion));
  context.output.insert(std::make_pair("U_HEAD.recordLength", std::to_string(header.recordLength)));

  context.output.insert(std::make_pair("U_HEAD.companyCode", Utility::getAlphanumeric(context.position, 4)));
  context.output.insert(std::make_pair("U_HEAD.uniqueTicketKey", Utility::getAlphanumeric(context.position, 20)));
  context.output.insert(std::make_pair("U_HEAD.editionTime", Utility::getAlphanumeric(context.position, 12))); // DDMMYYYYHHMM
  context.output.insert(std::make_pair("U_HEAD.flags", Utility::getAlphanumeric(context.position, 1)));
  context.output.insert(std::make_pair("U_HEAD.editionLanguageOfTicket", Utility::getAlphanumeric(context.position, 2)));
  context.output.insert(std::make_pair("U_HEAD.secondLanguageOfContract", Utility::getAlphanumeric(context.position, 2)));

  return context;
}
