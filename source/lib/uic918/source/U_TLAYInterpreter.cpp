
#include "../include/U_TLAYInterpreter.h"
#include "../include/Utility.h"
#include "../include/RCT2Field.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>

U_TLAYInterpreter::U_TLAYInterpreter(RecordHeader &&h) : header(std::move(h)) {}

Interpreter::Context &U_TLAYInterpreter::interpret(Context &context)
{
  if (header.recordId.compare("U_TLAY") != 0 || header.recordVersion.compare("01") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.to_string());
  }

  context.output.insert(std::make_pair("U_TLAY.recordId", header.recordId));
  context.output.insert(std::make_pair("U_TLAY.recordVersion", header.recordVersion));
  context.output.insert(std::make_pair("U_TLAY.recordLength", std::to_string(header.recordLength)));

  auto const layoutStandard = Utility::getAlphanumeric(context.position, 4);
  if (layoutStandard.compare("RCT2") != 0)
  {
    throw std::runtime_error(std::string("Unsupported layour: ") + layoutStandard);
  }

  context.output.insert(std::make_pair("U_TLAY.layoutStandard", layoutStandard));
  auto const numberOfFields = std::stoi(Utility::getAlphanumeric(context.position, 4));
  context.output.insert(std::make_pair("U_TLAY.numberOfFields", std::to_string(numberOfFields)));

  for (auto field = 0; field < numberOfFields && context.position != context.uncompressedMessage.end(); ++field)
  {
    auto header = RCT2Field(context.position);
    auto nameStream = std::stringstream();
    nameStream << "U_TLAY.field" << std::setw(4) << std::setfill('0') << field;
    context.output.insert(std::make_pair(nameStream.str(), header.fieldText));
    context.position = header.startPosition + header.recordLength;
  }

  return context;
}
