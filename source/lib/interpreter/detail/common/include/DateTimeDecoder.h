// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>

namespace interpreter::detail::common
{
    class Context;

    class DateTimeDecoder
    {
    public:
        /* Consumes 4 bytes and decodes date-time to ISO-8601 format
         */
        static std::string consumeDateTimeCompact4(Context &context);

        /* Consumes 12 bytes (ASCII) and decodes date-time to ISO-8601 format
         */
        static std::string consumeDateTime12(Context &context);

        /* Consumes 8 bytes (ASCII) and decodes date to ISO-8601 format
         */
        static std::string consumeDate8(Context &context);
    };
}
