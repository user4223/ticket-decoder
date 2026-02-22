// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/DateTimeDecoder.h"
#include "../include/StringDecoder.h"
#include "../include/InterpreterUtility.h"

#include <iomanip>
#include <sstream>

namespace interpreter::detail::common
{

    std::string DateTimeDecoder::consumeDateTimeCompact4(Context &context)
    {
        auto const date = common::consumeInteger2(context);
        auto const time = common::consumeInteger2(context);
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

    std::string DateTimeDecoder::consumeDateTime12(Context &context)
    {
        auto const input = StringDecoder::consumeASCII(context, 12, true);
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

    std::string DateTimeDecoder::consumeDate8(Context &context)
    {
        auto const input = StringDecoder::consumeASCII(context, 8, true);
        auto const p = input.begin();
        // TODO Use chrono parse or apply validation for all values
        std::ostringstream os; // DDMMYYYY
        os << std::string(p + 4, p + 8) << "-"
           << std::string(p + 2, p + 4) << "-"
           << std::string(p + 0, p + 2);
        return os.str();
    }
}
