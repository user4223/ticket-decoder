// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/CSVInterpreter.h"

#include "lib/interpreter/detail/common/include/StringDecoder.h"
#include "lib/interpreter/detail/common/include/DateTimeDecoder.h"
#include "lib/interpreter/detail/common/include/NumberDecoder.h"

#include "lib/infrastructure/include/Logging.h"

namespace interpreter::detail::misc
{
    constexpr std::vector<std::string_view> split(std::string_view str, char const delimiter)
    {
        auto parts = std::vector<std::string_view>{};
        auto position = std::size_t(0);
        while (position < str.size())
        {
            auto const nextDelimiter = str.find_first_of(delimiter, position);
            parts.push_back(str.substr(position, nextDelimiter - position));
            position = nextDelimiter == std::string_view::npos
                           ? str.size()
                           : nextDelimiter + 1;
        }
        return parts;
    }

    CSVInterpreter::CSVInterpreter(infrastructure::LoggerFactory &lf)
        : logger(CREATE_LOGGER(lf))
    {
    }

    bool CSVInterpreter::canInterpret(common::Context const &context) const
    {
        auto const content = context.peekRemainingBytes();
        if (!common::StringDecoder::isASCII(content, true))
        {
            return false;
        }

        return std::count(std::begin(content), std::end(content), ';') == 15;
    }

    common::Context CSVInterpreter::interpret(common::Context &&context)
    {
        auto const content = common::StringDecoder::decodeASCII(context.consumeRemainingBytes(), true);
        auto const parts = split(content, ';');
        auto const version = parts.empty() ? std::string("") : std::string(parts[0]);

        auto recordJson = utility::JsonBuilder::object();
        if (parts.size() == 16)
        {
            recordJson
                .add("price", common::NumberDecoder::decodeInteger(parts[3]))
                .add("validFromDate", common::DateTimeDecoder::decodeASCIIDate6(parts[5]))
                .add("validUntilDate", common::DateTimeDecoder::decodeASCIIDate6(parts[6]))
                .add("passengerNumber", common::NumberDecoder::decodeInteger(parts[7])) // maybe
                .add("ticketId", parts[8])                                              // maybe
                .add("issuingDate", common::DateTimeDecoder::decodeASCIIDate6(parts[11]))
                .add("fromStationIBNR", parts[12])
                .add("toStationIBNR", parts[13])
                .add("classCode", parts[15]); // maybe
        }

        context.addRecord(common::Record("CSV", version, std::move(recordJson)));
        context.addField("raw", content);
        context.addField("validated", "false");
        return context;
    }
}
