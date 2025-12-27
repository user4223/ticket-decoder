// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/RecordU_FLEX.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "../u_flex/v1.3/include/RecordU_FLEX_13.h"
#include "../u_flex/v3.0/include/RecordU_FLEX_30.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/infrastructure/include/Logging.h"

#include <sstream>
#include <string>
#include <optional>

namespace interpreter::detail::uic
{

  static std::map<std::string, std::function<std::optional<::utility::JsonBuilder>(infrastructure::Logger &, std::span<std::uint8_t const>)>> const uflexInterpreterMap = {
      {std::string("13"), u_flex13::convert},
      {std::string("03"), u_flex30::convert}};

  RecordU_FLEX::RecordU_FLEX(infrastructure::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("U_FLEX", {"13", "03"});
  }

  common::Context RecordU_FLEX::interpret(common::Context &&context)
  {
    auto const asn1UperBytes = header.consumeRemainingRecordBytes(context);
    auto const uflexInterpreter = uflexInterpreterMap.at(header.recordVersion);
    auto recordJson = uflexInterpreter(logger, asn1UperBytes);

    if (!recordJson)
    {
      return std::move(context);
    }

    context.addRecord(common::Record(header.recordId, header.recordVersion, std::move(*recordJson)));
    return std::move(context);
  }
}
