
#include "../include/RecordInterpreterU_TLAY.h"
#include "../include/Utility.h"
#include "../include/RCT2Field.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>

RecordInterpreterU_TLAY::RecordInterpreterU_TLAY(RecordHeader &&h) : header(std::move(h)) {}

Context &RecordInterpreterU_TLAY::interpret(Context &context)
{
  if (header.recordId.compare("U_TLAY") != 0 || header.recordVersion.compare("01") != 0)
  {
    throw std::runtime_error(std::string("Unsupported header: ") + header.toString());
  }

  context.addField("U_TLAY.recordId", header.recordId);
  context.addField("U_TLAY.recordVersion", header.recordVersion);
  context.addField("U_TLAY.recordLength", std::to_string(header.recordLength));

  auto const layoutStandard = Utility::getAlphanumeric(context.getPosition(), 4);
  if (layoutStandard.compare("RCT2") != 0)
  {
    throw std::runtime_error(std::string("Unsupported layour: ") + layoutStandard);
  }

  context.addField("U_TLAY.layoutStandard", layoutStandard);
  auto const numberOfFields = std::stoi(Utility::getAlphanumeric(context.getPosition(), 4));
  context.addField("U_TLAY.numberOfFields", std::to_string(numberOfFields));

  for (auto fieldIndex = 0; fieldIndex < numberOfFields && !context.isEmpty(); ++fieldIndex)
  {
    auto const field = RCT2Field{context.getPosition()};

    auto nameStream = std::stringstream();
    nameStream << "U_TLAY.field" << std::setw(4) << std::setfill('0') << fieldIndex;
    context.addField(nameStream.str(), field.text, field.getLayoutString());
  }

  return context;
}
