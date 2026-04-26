// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <cstdint>
#include <span>
#include <optional>

namespace interpreter::detail::common
{
    class Context;

    class DateTimeDecoder
    {
    public:
        /* Consumes 4 bytes and decodes date-time-compact format to
           ISO-8601 formatted string.
         */
        static std::string consumeDateTimeCompact4(Context &context);
        static std::string decodeDateTimeCompact4(std::span<std::uint8_t const> bytes);

        /* Consumes 12 bytes (ASCII - DDMMYYYYHHMM) and decodes date-time to
           ISO-8601 formatted string.
         */
        static std::string consumeASCIIDateTime12(Context &context);

        /* Consumes 8 bytes (ASCII - DDMMYYYY) and decodes date to
           ISO-8601 formatted string.
         */
        static std::string consumeASCIIDate8(Context &context);

        /* Consumes 6 bytes (ASCII - DDMMYY) and decodes date to
           ISO-8601 formatted string.
         */
        static std::string consumeASCIIDate6(Context &context);
        static std::string decodeASCIIDate6(std::string_view const &input);

        /* Consumes 4 bytes (BCD - YYYYMMDD) and decodes date
           to ISO-8601 formatted string.
           Returns an empty optional when all bytes are 0.
         */
        static std::optional<std::string> consumeBCDDate4(Context &context);
    };
}
