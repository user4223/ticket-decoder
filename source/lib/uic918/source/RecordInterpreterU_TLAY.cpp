
#include "../include/RecordInterpreterU_TLAY.h"
#include "../include/Utility.h"
#include "../include/RCT2Field.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>

RecordInterpreterU_TLAY::RecordInterpreterU_TLAY(RecordHeader &&h) : header(std::move(h))
{
  header.ensure("U_TLAY", {"01"});
}

Context &RecordInterpreterU_TLAY::interpret(Context &context)
{
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