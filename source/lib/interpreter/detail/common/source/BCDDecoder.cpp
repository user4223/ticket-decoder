// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/BCDDecoder.h"

#include "lib/interpreter/detail/common/include/Context.h"

#include <stdexcept>

namespace interpreter::detail::common
{

    std::uint16_t BCDDecoder::consumePackedInteger2(Context &context)
    {
        return decodePackedInteger2(context.consumeBytes(2));
    }

    std::uint16_t BCDDecoder::decodePackedInteger2(std::span<std::uint8_t const> bytes)
    {
        if (bytes.size() < 2) {
            throw std::runtime_error(std::string("Less than expected bytes available, expecting at least: 2"));
        }
        std::uint16_t const high = decodePackedInteger1(bytes[0]);
        std::uint16_t const low = decodePackedInteger1(bytes[1]);
        return high * 100 + low;
    }

    std::uint8_t BCDDecoder::consumePackedInteger1(Context &context)
    {
        return decodePackedInteger1(context.consumeByte());
    }

    std::uint8_t BCDDecoder::decodePackedInteger1(std::uint8_t byte)
    {
        std::uint8_t const high = byte >> 4 & 0x0F;
        std::uint8_t const low = byte & 0x0F;
        return high * 10 + low;
    }
}
