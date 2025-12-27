// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/RecordU_HEAD.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"
#include "lib/interpreter/detail/common/include/Record.h"

#include "lib/utility/include/JsonBuilder.h"
#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::uic
{
  RecordU_HEAD::RecordU_HEAD(infrastructure::LoggerFactory &loggerFactory, RecordHeader &&h)
      : AbstractRecord(CREATE_LOGGER(loggerFactory), std::move(h))
  {
    header.ensure("U_HEAD", {"01"});
  }

  common::Context RecordU_HEAD::interpret(common::Context &&context)
  {
    auto recordJson = ::utility::JsonBuilder::object();
    recordJson
        .add("companyCode", common::getAlphanumeric(context, 4))
        .add("uniqueTicketKey", common::getAlphanumeric(context, 20))
        .add("editionTime", common::getDateTime12(context))
        .add("flags", common::getAlphanumeric(context, 1))
        .add("editionLanguageOfTicket", common::getAlphanumeric(context, 2))
        .add("secondLanguageOfContract", common::getAlphanumeric(context, 2));

    context.addRecord(common::Record(header.recordId, header.recordVersion, std::move(recordJson)));
    return std::move(context);
  }
}
