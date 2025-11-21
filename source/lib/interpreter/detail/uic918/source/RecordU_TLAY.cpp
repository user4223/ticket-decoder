
#include "../include/RecordU_TLAY.h"

#include "lib/interpreter/detail/common/include/Utility.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace interpreter::detail::uic
{

  RecordU_TLAY::RecordU_TLAY(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("U_TLAY", {"01"});
  }

  Context RecordU_TLAY::interpret(Context &&context)
  {
    auto const layoutStandard = getAlphanumeric(context.getPosition(), 4);
    context.addField("U_TLAY.layoutStandard", layoutStandard);
    if (layoutStandard.compare("RCT2") != 0 && layoutStandard.compare("PLAI") != 0)
    {
      getBytes(context.getPosition(), header.getRemaining(context.getPosition()));
      LOG_WARN(logger) << "Unknown layout standard found: " << layoutStandard;
      return std::move(context);
    }

    auto recordJson = ::utility::JsonBuilder::object(); // clang-format off
    recordJson
      .add("fields", ::utility::toArray(std::stoi(getAlphanumeric(context.getPosition(), 4)), [&](auto &builder)
        { builder
            .add("line", std::stoi(getAlphanumeric(context.getPosition(), 2)))
            .add("column", std::stoi(getAlphanumeric(context.getPosition(), 2)))
            .add("height", std::stoi(getAlphanumeric(context.getPosition(), 2)))
            .add("width", std::stoi(getAlphanumeric(context.getPosition(), 2)))
            .add("formatting", getAlphanumeric(context.getPosition(), 1));

          auto const length = std::stoi(getAlphanumeric(context.getPosition(), 4));
          builder
            .add("text", getAlphanumeric(context.getPosition(), length));
        })); // clang-format on

    context.addRecord(Record(header.recordId, header.recordVersion, std::move(recordJson)));
    return std::move(context);
  }
}
