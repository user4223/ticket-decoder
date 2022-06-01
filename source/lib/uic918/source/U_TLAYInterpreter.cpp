
#include "../include/U_TLAYInterpreter.h"
#include "../include/Utility.h"
#include "../include/BlockHeader.h"

#include <stdexcept>

U_TLAYInterpreter::U_TLAYInterpreter(BlockHeader &&h) : header(std::move(h)) {}

Interpreter::Context &U_TLAYInterpreter::interpret(Context &context)
{
  if (header.recordId.compare("U_TLAY") != 0 || header.recordVersion.compare("01") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.to_string());
  }

  context.output.insert(std::make_pair("U_TLAY.recordId", header.recordId));
  context.output.insert(std::make_pair("U_TLAY.recordVersion", header.recordVersion));
  context.output.insert(std::make_pair("U_TLAY.recordLength", std::to_string(header.recordLength)));

  context.output.insert(std::make_pair("U_TLAY.layoutStandard", Utility::getAlphanumeric(context.position, 4)));
  context.output.insert(std::make_pair("U_TLAY.numberOfFields", Utility::getAlphanumeric(context.position, 4)));

  return context;
}
