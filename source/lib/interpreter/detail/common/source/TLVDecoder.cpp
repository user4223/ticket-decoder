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

    TLVTag TLVDecoder::consumeTag(common::Context &context)
    {
        auto tag = TLVTag{};
        tag.assign(0, context.consumeByte());
        // auto const usage = (first & 0xC0) >> 6; // 0 universal, 1 application, 2 context-specific, 3 private
        // auto const type = (first & 0x20) >> 5; // 0 primitive, 1 constructed
        // auto const tag = (first & 0x1F); // 0b11111 (31) see further bytes or else single byte tag value

        if ((tag[0] & 0x1F) == 0x1F)
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

    common::Context &TLVDecoder::consumeExpectedTag(common::Context &context, common::TLVTag const &expectedTag)
    {
        consumeTag(context).ensureEqual(expectedTag);
        return context;
    }

    std::size_t TLVDecoder::consumeLength(common::Context &context)
    {
        auto const first = context.consumeByte();
        if (first < 0x80)
        {
            return first;
        }

        auto const maxSize = sizeof(std::size_t);
        auto const length = first & 0x7f;
        if (length > maxSize)
        {
            throw std::runtime_error(std::string("Expecting ") + std::to_string(maxSize) + " as a max no of successor bytes for length, found unsupported length: " + std::to_string(length));
        }

        auto const source = context.consumeBytes(length);
        auto result = std::size_t(0);
        auto const destination = std::span<std::uint8_t>(reinterpret_cast<std::uint8_t *>(&result), maxSize);
        if constexpr (std::endian::native == std::endian::little)
        {
            std::copy(source.rbegin(), source.rend(), destination.begin());
        }
        else
        {
            std::copy(source.begin(), source.end(), destination.begin() + maxSize - length);
        }
        return result;
    }

    std::span<std::uint8_t const> TLVDecoder::consumeExpectedElement(common::Context &context, common::TLVTag const &expectedTag)
    {
        return context.consumeBytes(consumeLength(consumeExpectedTag(context, expectedTag)));
    }
}
