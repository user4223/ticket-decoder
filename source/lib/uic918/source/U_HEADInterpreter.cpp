
#include "../include/U_HEADInterpreter.h"
#include "../include/Utility.h"

#include <iterator>

Interpreter::Context &U_HEADInterpreter::interpret(Context &context)
{
  if (std::distance(context.position, context.uncompressedMessage.cend()) < 8)
  {
    return context;
  }

  auto const recordId = Utility::getAlphanumeric(context.position, 6);
  auto const recordVersion = Utility::getAlphanumeric(context.position, 2);
  if (recordId.compare("U_HEAD") != 0 || recordVersion.compare("01") != 0)
  {
    return context;
  }
  context.output.insert(std::make_pair("U_HEAD.recordId", recordId));
  context.output.insert(std::make_pair("U_HEAD.recordVersion", recordVersion));
  context.output.insert(std::make_pair("U_HEAD.recordLength", Utility::getAlphanumeric(context.position, 4)));
  context.output.insert(std::make_pair("U_HEAD.companyCode", Utility::getAlphanumeric(context.position, 4)));
  context.output.insert(std::make_pair("U_HEAD.unambiguousTicketKey", Utility::getAlphanumeric(context.position, 20)));
  context.output.insert(std::make_pair("U_HEAD.editionTime", Utility::getAlphanumeric(context.position, 12))); // DDMMYYYYHHMM
  context.output.insert(std::make_pair("U_HEAD.flags", Utility::getAlphanumeric(context.position, 1)));
  context.output.insert(std::make_pair("U_HEAD.editionLanguageOfTicket", Utility::getAlphanumeric(context.position, 2)));
  context.output.insert(std::make_pair("U_HEAD.secondLanguageOfContract", Utility::getAlphanumeric(context.position, 2)));

  return context;
}
