// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/CSVInterpreter.h"

#include "lib/interpreter/detail/common/include/StringDecoder.h"
#include "lib/interpreter/detail/common/include/DateTimeDecoder.h"
#include "lib/interpreter/detail/common/include/NumberDecoder.h"

#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::misc
{
    void split(std::string_view str, char const delimiter, std::insert_iterator<std::vector<std::string_view>> &&inserter)
    {
        auto pos = std::size_t(0);
        while (pos < str.size())
        {
            auto const next_pos = str.find_first_of(delimiter, pos);
            inserter = str.substr(pos, next_pos - pos);
            pos = next_pos == std::string_view::npos
                      ? str.size()
                      : next_pos + 1;
        }
    }

    CSVInterpreter::CSVInterpreter(infrastructure::LoggerFactory &lf)
        : logger(CREATE_LOGGER(lf))
    {
    }

    bool CSVInterpreter::canInterpret(common::Context const &context) const
    {
        auto const content = context.peekRemainingBytes();
        if (!common::StringDecoder::ensureASCII(content, true))
        {
            return false;
        }

        return std::count(std::begin(content), std::end(content), ';') == 15;
    }

    common::Context CSVInterpreter::interpret(common::Context &&context)
    {
        auto const content = common::StringDecoder::decodeASCII(context.consumeRemainingBytes(), true);

        auto parts = std::vector<std::string_view>{};
        split(content, ';', std::inserter(parts, parts.begin()));

        auto recordJson = utility::JsonBuilder::object();
        if (parts.size() == 16)
        {
            auto const price = common::NumberDecoder::decodeInteger(parts[3]);
            auto const validFromDate = common::DateTimeDecoder::decodeDate6(parts[5]);
            auto const validUntilDate = common::DateTimeDecoder::decodeDate6(parts[6]);
            auto const passengerNumber = common::NumberDecoder::decodeInteger(parts[7]);
            auto const ticketId = parts[8];
            auto const issuingDate = common::DateTimeDecoder::decodeDate6(parts[11]);
            auto const fromStationIBNR = parts[12];
            auto const toStationIBNR = parts[13];
            auto const classCode = parts[15];
        }

        context.addRecord(common::Record("CSV", "", std::move(recordJson)));
        context.addField("raw", content);
        context.addField("validated", "false");
        return context;
    }
}
