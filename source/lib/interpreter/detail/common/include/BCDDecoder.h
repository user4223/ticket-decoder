// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdint>
#include <span>
#include <string>

namespace interpreter::detail::common
{
    class Context;

    /* Decoder for binary coded decimal numbers (BCD).
     */
    class BCDDecoder
    {
    public:
        /* Consumes 2 bytes in big endian order and decodes packed binary coded decimal value
         */
        static std::uint16_t consumePackedInteger2(Context &context);
        static std::string consumePackedInteger2AsString(Context &context);

        /* Decodes 2 bytes in big endian order and decodes packed binary coded decimal value
         */
        static std::uint16_t decodePackedInteger2(std::span<std::uint8_t const> bytes);
        static std::string decodePackedInteger2AsString(std::span<std::uint8_t const> bytes);

        /* Consumes 1 byte and decodes packed binary coded decimal value
         */
        static std::uint8_t consumePackedInteger1(Context &context);
        static std::string consumePackedInteger1AsString(Context &context);

        /* Decodes 1 byte and decodes packed binary coded decimal value
         */
        static std::uint8_t decodePackedInteger1(std::uint8_t byte);
        static std::string decodePackedInteger1AsString(std::uint8_t byte);
    };
}
