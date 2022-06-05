
#include "../include/RCT2Field.h"
#include "../include/Utility.h"

#include <sstream>

RCT2Field::RCT2Field(Interpreter::BytesType::const_iterator &position)
    : fieldLine(std::stoi(Utility::getAlphanumeric(position, 2))),
      fieldColumn(std::stoi(Utility::getAlphanumeric(position, 2))),
      fieldHeight(std::stoi(Utility::getAlphanumeric(position, 2))),
      fieldWidth(std::stoi(Utility::getAlphanumeric(position, 2))),
      fieldFormatting(Utility::getAlphanumeric(position, 1)),
      fieldTextLength(std::stoi(Utility::getAlphanumeric(position, 4))),
      fieldText(Utility::getAlphanumeric(position, fieldTextLength))
{
}

std::string RCT2Field::to_string()
{
  auto stream = std::ostringstream{};
  stream << "[" << fieldLine << "," << fieldColumn
         << "|" << fieldWidth << "," << fieldHeight
         << "|" << fieldFormatting << "] "
         << fieldText;
  return stream.str();
}
