// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/TLVDecoder.h"

#include "lib/interpreter/detail/common/include/Context.h"

#include <sstream>

namespace interpreter::detail::common
{

    void TLVTag::ensureEqual(TLVTag const &rhs) const
    {
        if (*this != rhs)
        {
            throw std::runtime_error(std::string("Unexpected tag found: ") + toHexString());
        }
    }

    std::string TLVTag::toHexString() const
    {
        std::stringstream os;
        auto const bytes = std::span<std::uint8_t const>(getByte(0), size());
        std::for_each(std::begin(bytes), std::end(bytes), [&](auto const &byte)
                      { os << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)byte; });
        return os.str();
    }

    constexpr bool TLVDecoder::hasSuccessor(std::uint8_t const &value)
    {
        return (value & 0x1F) == 0x1F;
    }

    TLVTag TLVDecoder::consumeTag(common::Context &context)
    {
        auto tag = TLVTag{0, 0, 0, 0};
        tag.assign(0, context.consumeByte());
        // auto const usage = (first & 0xC0) >> 6; // 0 universal, 1 application, 2 context-specific, 3 private
        // auto const type = (first & 0x20) >> 5; // 0 primitive, 1 constructed
        // auto const tag = (first & 0x1F); // 0b11111 (31) see further bytes or else single byte tag value

        if (hasSuccessor(tag[0]))
        {
            for (int index = 1; index < tag.maximumSize(); index++)
            {
                auto const byte = context.consumeByte();
                tag.assign(index, byte);
                if ((byte & 0x80) == 0)
                {
                    break;
                }
            }
        }

        return tag;
    }
}
