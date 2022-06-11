
#include "../include/RCT2Field.h"
#include "../include/Utility.h"

#include <sstream>

RCT2Field::RCT2Field(Context::BytesType::const_iterator &position)
    : line(std::stoi(Utility::getAlphanumeric(position, 2))),
      column(std::stoi(Utility::getAlphanumeric(position, 2))),
      height(std::stoi(Utility::getAlphanumeric(position, 2))),
      width(std::stoi(Utility::getAlphanumeric(position, 2))),
      formatting(Utility::getAlphanumeric(position, 1)),
      length(std::stoi(Utility::getAlphanumeric(position, 4))),
      text(Utility::getAlphanumeric(position, length))
{
}

std::string RCT2Field::getLayoutString() const
{
  auto stream = std::ostringstream{};
  stream << "L" << line << ",C" << column
         << ",W" << width << ",H" << height
         << ",F" << formatting;
  return stream.str();
}
