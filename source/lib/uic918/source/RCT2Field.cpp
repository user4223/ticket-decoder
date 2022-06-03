
#include "../include/RCT2Field.h"
#include "../include/Utility.h"

RCT2Field::RCT2Field(Interpreter::BytesType::const_iterator &position)
    : startPosition(position),
      fieldLine(Utility::getAlphanumeric(position, 2)),
      fieldColumn(Utility::getAlphanumeric(position, 2)),
      fieldHeight(Utility::getAlphanumeric(position, 2)),
      fieldWidth(Utility::getAlphanumeric(position, 2)),
      fieldFormatting(Utility::getAlphanumeric(position, 1)),
      fieldTextLength(Utility::getAlphanumeric(position, 4)),
      fieldText(Utility::getAlphanumeric(position, std::stoi(fieldTextLength))),
      recordLength(13 + std::stoi(fieldTextLength))
{
}
