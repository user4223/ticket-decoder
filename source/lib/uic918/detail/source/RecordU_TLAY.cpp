
#include "../include/RecordU_TLAY.h"
#include "../include/Utility.h"

#include "../../api/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace uic918::detail
{

  RecordU_TLAY::RecordU_TLAY(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("U_TLAY", {"01"});
  }

  Context &RecordU_TLAY::interpret(Context &context)
  {
    auto const layoutStandard = utility::getAlphanumeric(context.getPosition(), 4);
    context.addField("U_TLAY.layoutStandard", layoutStandard);
    if (layoutStandard.compare("RCT2") != 0 && layoutStandard.compare("PLAI") != 0)
    {
      utility::getBytes(context.getPosition(), header.getRemaining(context.getPosition()));
      LOG_WARN(logger) << "Unknown layout standard found: " << layoutStandard;
      return context;
    }

    auto recordJson = ::utility::JsonBuilder::object() // clang-format off
      .add("fields", ::utility::toArray(std::stoi(utility::getAlphanumeric(context.getPosition(), 4)),
        [&]()
        {
          auto field = ::utility::JsonBuilder::object()
            .add("line", std::stoi(utility::getAlphanumeric(context.getPosition(), 2)))
            .add("column", std::stoi(utility::getAlphanumeric(context.getPosition(), 2)))
            .add("height", std::stoi(utility::getAlphanumeric(context.getPosition(), 2)))
            .add("width", std::stoi(utility::getAlphanumeric(context.getPosition(), 2)))
            .add("formatting", utility::getAlphanumeric(context.getPosition(), 1));

          auto const length = std::stoi(utility::getAlphanumeric(context.getPosition(), 4));
          return field
            .add("text", utility::getAlphanumeric(context.getPosition(), length));
        })); // clang-format on

    return context.addRecord(api::Record(header.recordId, header.recordVersion, recordJson.build()));
  }
}
