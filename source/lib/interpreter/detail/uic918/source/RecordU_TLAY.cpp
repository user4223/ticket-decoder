// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/RecordU_TLAY.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
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

  common::Context RecordU_TLAY::interpret(common::Context &&context)
  {
    auto const layoutStandard = common::getAlphanumeric(context.getPosition(), 4);
    context.addField("U_TLAY.layoutStandard", layoutStandard);
    if (layoutStandard.compare("RCT2") != 0 && layoutStandard.compare("PLAI") != 0)
    {
      common::getBytes(context.getPosition(), header.getRemaining(context.getPosition()));
      LOG_WARN(logger) << "Unknown layout standard found: " << layoutStandard;
      return std::move(context);
    }

    auto recordJson = ::utility::JsonBuilder::object(); // clang-format off
    recordJson
      .add("fields", ::utility::toArray(std::stoi(common::getAlphanumeric(context.getPosition(), 4)), [&](auto &builder)
        { builder
            .add("line", std::stoi(common::getAlphanumeric(context.getPosition(), 2)))
            .add("column", std::stoi(common::getAlphanumeric(context.getPosition(), 2)))
            .add("height", std::stoi(common::getAlphanumeric(context.getPosition(), 2)))
            .add("width", std::stoi(common::getAlphanumeric(context.getPosition(), 2)))
            .add("formatting", common::getAlphanumeric(context.getPosition(), 1));

          auto const length = std::stoi(common::getAlphanumeric(context.getPosition(), 4));
          builder
            .add("text", common::getAlphanumeric(context.getPosition(), length));
        })); // clang-format on

    context.addRecord(common::Record(header.recordId, header.recordVersion, std::move(recordJson)));
    return std::move(context);
  }
}
