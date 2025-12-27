// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Record118199.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::uic
{

  Record118199::Record118199(infrastructure::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("118199", {"01"});
  }

  common::Context Record118199::interpret(common::Context &&context)
  {
    auto const jsonString = common::getAlphanumeric(context, context.getRemainingSize());

    context.addRecord(common::Record(header.recordId, header.recordVersion, ::utility::JsonBuilder(jsonString)));
    return std::move(context);
  }
}
