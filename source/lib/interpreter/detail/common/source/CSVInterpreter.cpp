// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/CSVInterpreter.h"
#include "../include/StringDecoder.h"

#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::common
{

    CSVInterpreter::CSVInterpreter(infrastructure::LoggerFactory &lf)
        : logger(CREATE_LOGGER(lf))
    {
    }

    bool CSVInterpreter::canInterpret(common::Context const &context) const
    {
        return false;
    }

    common::Context CSVInterpreter::interpret(common::Context &&context)
    {
        auto const content = StringDecoder::decodeASCII(context.consumeRemainingBytes(), true);

        context.addRecord(common::Record("VAR", "", utility::JsonBuilder()));
        context.addField("raw", content);
        context.addField("validated", "false");
        return context;
    }
}
