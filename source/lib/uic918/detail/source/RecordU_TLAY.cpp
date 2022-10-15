
#include "../include/RecordU_TLAY.h"
#include "../include/Utility.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace uic918::detail
{
  struct RCT2Field : Interpreter
  {
    std::string prefix;

    RCT2Field(std::string p) : prefix(p) {}

    virtual Context &interpret(Context &context) override
    {
      auto const line = std::stoi(utility::getAlphanumeric(context.getPosition(), 2));
      auto const column = std::stoi(utility::getAlphanumeric(context.getPosition(), 2));
      auto const height = std::stoi(utility::getAlphanumeric(context.getPosition(), 2));
      auto const width = std::stoi(utility::getAlphanumeric(context.getPosition(), 2));
      auto const formatting = utility::getAlphanumeric(context.getPosition(), 1);
      auto formatStream = std::ostringstream{};
      formatStream << "L" << line << ", C" << column << ", "
                   << "W" << width << ", H" << height << ", "
                   << "F" << formatting;
      context.addField(prefix + "format", formatStream.str());

      auto const length = std::stoi(utility::getAlphanumeric(context.getPosition(), 4));
      auto const text = utility::getAlphanumeric(context.getPosition(), length);
      context.addField(prefix + "text", text);
      return context;
    }
  };

  RecordInterpreterU_TLAY::RecordInterpreterU_TLAY(RecordHeader &&h)
      : AbstractRecord(std::move(h))
  {
    header.ensure("U_TLAY", {"01"});
  }

  Context &RecordInterpreterU_TLAY::interpret(Context &context)
  {
    auto const layoutStandard = utility::getAlphanumeric(context.getPosition(), 4);
    context.addField("U_TLAY.layoutStandard", layoutStandard);
    if (layoutStandard.compare("RCT2") != 0 && layoutStandard.compare("PLAI") != 0)
    {
      utility::getBytes(context.getPosition(), header.getRemaining(context.getPosition()));
      return context;
    }

    auto const numberOfFields = std::stoi(utility::getAlphanumeric(context.getPosition(), 4));
    context.addField("U_TLAY.numberOfFields", std::to_string(numberOfFields));

    for (auto fieldIndex = 0; fieldIndex < numberOfFields && !context.isEmpty(); ++fieldIndex)
    {
      auto nameStream = std::stringstream();
      nameStream << "U_TLAY.field" << std::setw(4) << std::setfill('0') << fieldIndex << ".";
      RCT2Field{nameStream.str()}.interpret(context);
    }

    return context;
  }
}
