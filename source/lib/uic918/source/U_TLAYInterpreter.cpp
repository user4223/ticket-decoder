
#include "../include/U_TLAYInterpreter.h"
#include "../include/Utility.h"

#include <iterator>

Interpreter::Context &U_TLAYInterpreter::interpret(Context &context)
{
  if (std::distance(context.position, context.uncompressedMessage.cend()) < 8)
  {
    return context;
  }

  auto const recordId = Utility::getAlphanumeric(context.position, 6);
  auto const recordVersion = Utility::getAlphanumeric(context.position, 2);
  if (recordId.compare("U_TLAY") != 0 || recordVersion.compare("01") != 0)
  {
    return context;
  }
  context.output.insert(std::make_pair("U_TLAY.recordId", recordId));
  context.output.insert(std::make_pair("U_TLAY.recordVersion", recordVersion));
  context.output.insert(std::make_pair("U_TLAY.recordLength", Utility::getAlphanumeric(context.position, 4)));
  context.output.insert(std::make_pair("U_TLAY.layoutStandard", Utility::getAlphanumeric(context.position, 4)));
  context.output.insert(std::make_pair("U_TLAY.numberOfFields", Utility::getAlphanumeric(context.position, 4)));

  return context;
}
