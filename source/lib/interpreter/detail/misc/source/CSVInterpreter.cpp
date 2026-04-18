// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/CSVInterpreter.h"

#include "lib/interpreter/detail/common/include/StringDecoder.h"

#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::misc
{

    CSVInterpreter::CSVInterpreter(infrastructure::LoggerFactory &lf)
        : logger(CREATE_LOGGER(lf))
    {
    }

    bool CSVInterpreter::canInterpret(common::Context const &context) const
    {
        // TODO Ensure printable ASCII only and ensure CSV format containing separators
        return false;
    }

    common::Context CSVInterpreter::interpret(common::Context &&context)
    {
        auto const content = common::StringDecoder::decodeASCII(context.consumeRemainingBytes(), true);

        // TODO Split and interprete parts when known

        context.addRecord(common::Record("CSV", "", utility::JsonBuilder()));
        context.addField("raw", content);
        context.addField("validated", "false");
        return context;
    }
}
