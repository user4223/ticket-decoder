
#include "../include/RecordU_FLEX.h"
#include "../include/Utility.h"

#include "lib/interpreter/detail/common/include/Record.h"

#include "../u_flex/v1.3/include/RecordU_FLEX_13.h"
#include "../u_flex/v3.0/include/RecordU_FLEX_30.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/utility/include/Logging.h"

#include <sstream>
#include <string>
#include <optional>

namespace uic918::detail
{

  static std::map<std::string, std::function<std::optional<::utility::JsonBuilder>(::utility::Logger &, std::vector<std::uint8_t> const &)>> const uflexInterpreterMap = {
      {std::string("13"), u_flex13::convert},
      {std::string("03"), u_flex30::convert}};

  RecordU_FLEX::RecordU_FLEX(::utility::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("U_FLEX", {"13", "03"});
  }

  Context RecordU_FLEX::interpret(Context &&context)
  {
    auto const asn1UperBytes = utility::getBytes(context.getPosition(), header.getRemaining(context.getPosition()));
    auto const uflexInterpreter = uflexInterpreterMap.at(header.recordVersion);
    auto recordJson = uflexInterpreter(logger, asn1UperBytes);

    if (!recordJson)
    {
      return std::move(context);
    }

    context.addRecord(api::Record(header.recordId, header.recordVersion, std::move(*recordJson)));
    return std::move(context);
  }
}
