// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/DateTimeDecoder.h"
#include "../include/StringDecoder.h"
#include "../include/NumberDecoder.h"
#include "../include/BCDDecoder.h"
#include "../include/Context.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace interpreter::detail::common
{
    static void ensureInputLength(std::string_view const &input, std::size_t expected)
    {
        if (input.size() != expected)
        {
            throw std::runtime_error("Unexpected date input length: expected " + std::to_string(expected) + ", found " + std::to_string(input.size()));
        }
    }

    std::string DateTimeDecoder::consumeDateTimeCompact4(Context &context)
    {
        auto const date = NumberDecoder::consumeUInteger2(context);
        auto const time = NumberDecoder::consumeUInteger2(context);

        if (date == 0 && time == 0)
        {
            return {"0000-00-00T00:00:00"};
        }

        // TODO Use chrono parse or apply validation for all values
        std::ostringstream os;
        os << std::setw(4) << std::setfill('0') << std::to_string(((date & 0xFE00) >> 9) + 1990) << "-"
           << std::setw(2) << std::setfill('0') << std::to_string(((date & 0x01E0) >> 5)) << "-"
           << std::setw(2) << std::setfill('0') << std::to_string(((date & 0x001F) >> 0)) << "T"
           << std::setw(2) << std::setfill('0') << std::to_string(((time & 0xF800) >> 11)) << ":"
           << std::setw(2) << std::setfill('0') << std::to_string(((time & 0x07E0) >> 5)) << ":"
           << std::setw(2) << std::setfill('0') << std::to_string(((time & 0x001F) >> 0));
        return os.str();
    }

    std::string DateTimeDecoder::decodeDateTimeCompact4(std::span<std::uint8_t const> bytes)
    {
        auto context = Context(bytes);
        return consumeDateTimeCompact4(context);
    }

    std::string DateTimeDecoder::consumeASCIIDateTime12(Context &context)
    {
        context.ensureRemaining(12);
        auto const input = StringDecoder::consumeASCII(context, 12, true);
        ensureInputLength(input, 12);
        auto const p = input.begin();
        // TODO Use chrono parse or apply validation for all values
        std::ostringstream os; // DDMMYYYYHHMM
        os << std::string(p + 4, p + 8) << "-"
           << std::string(p + 2, p + 4) << "-"
           << std::string(p + 0, p + 2) << "T"
           << std::string(p + 8, p + 10) << ":"
           << std::string(p + 10, p + 12) << ":"
           << "00";
        return os.str();
    }

    std::string DateTimeDecoder::consumeASCIIDate8(Context &context)
    {
        context.ensureRemaining(8);
        auto const input = StringDecoder::consumeASCII(context, 8, true);
        ensureInputLength(input, 8);
        auto const p = input.begin();
        // TODO Use chrono parse or apply validation for all values
        std::ostringstream os; // DDMMYYYY
        os << std::string(p + 4, p + 8) << "-"
           << std::string(p + 2, p + 4) << "-"
           << std::string(p + 0, p + 2);
        return os.str();
    }

    std::string DateTimeDecoder::consumeASCIIDate6(Context &context)
    {
        context.ensureRemaining(6);
        auto const input = StringDecoder::consumeASCII(context, 6, true);
        return decodeASCIIDate6(input);
    }

    std::string DateTimeDecoder::decodeASCIIDate6(std::string_view const &input)
    {
        ensureInputLength(input, 6);
        if (input == std::string("000000"))
        {
            return "0000-00-00";
        }

        auto const p = input.begin();
        std::ostringstream os; // DDMMYY
        os << 20 << std::string(p + 4, p + 6) << "-"
           << std::string(p + 2, p + 4) << "-"
           << std::string(p + 0, p + 2);
        return os.str();
    }

    std::optional<std::string> DateTimeDecoder::consumeBCDDate4(Context &context)
    {
        auto const year = BCDDecoder::consumePackedInteger2(context);
        auto const month = BCDDecoder::consumePackedInteger1(context);
        auto const day = BCDDecoder::consumePackedInteger1(context);

        if (year == 0 && month == 0 && day == 0)
        {
            return {};
        }

        std::ostringstream os;
        os << std::setw(4) << std::setfill('0') << std::to_string(year) << "-"
           << std::setw(2) << std::setfill('0') << std::to_string(month) << "-"
           << std::setw(2) << std::setfill('0') << std::to_string(day);

        return std::make_optional(os.str());
    }
}
