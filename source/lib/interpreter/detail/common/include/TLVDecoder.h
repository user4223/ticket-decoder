// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdint>
#include <string>
#include <initializer_list>
#include <span>
#include <map>
#include <functional>

namespace interpreter::detail::common
{
    class Context;

    class TLVTag
    {
        std::uint32_t value; // this is used in big endian byte order, so do not access it directly or be confused on little endian machines
        std::size_t currentSize;

        constexpr std::uint8_t *getByte(std::size_t index) const { return ((std::uint8_t *)&value) + index; }

    public:
        constexpr TLVTag() : value(0), currentSize(0) {}
        constexpr TLVTag(std::initializer_list<std::uint8_t> const bytes) : TLVTag::TLVTag()
        {
            auto const *source = bytes.begin();
            auto *destination = getByte(0);
            std::size_t index = 0;
            for (; index < bytes.size() && index < maximumSize(); ++index)
            {
                destination[index] = source[index];
            }
            currentSize = index;
        }

        TLVTag(TLVTag const &) = default;
        TLVTag(TLVTag &&) = default;

        TLVTag &operator=(TLVTag const &) = default;
        TLVTag &operator=(TLVTag &&) = default;

        constexpr void assign(std::size_t index, std::uint8_t value)
        {
            if (index + 1 > currentSize)
            {
                currentSize = index + 1;
            }
            *getByte(index) = value;
        }

        constexpr std::uint8_t const &operator[](std::size_t index) const { return *getByte(index); }

        constexpr std::size_t size() const { return currentSize; }

        constexpr std::size_t maximumSize() const { return sizeof(decltype(value)); }

        constexpr bool operator==(TLVTag const &rhs) const { return currentSize == rhs.currentSize && value == rhs.value; }

        constexpr bool operator!=(TLVTag const &rhs) const { return currentSize != rhs.currentSize || value != rhs.value; }

        constexpr bool operator<(TLVTag const &rhs) const { return currentSize < rhs.currentSize && value < rhs.value; };

        void ensureEqual(TLVTag const &rhs) const;

        std::string toHexString() const;
    };

    class TLVDecoder
    {
        using TagMapType = std::map<TLVTag, std::function<void(std::span<std::uint8_t const>)>>;

        TagMapType const tagMap;

    public:
        TLVDecoder(TagMapType tagMap);

        std::size_t consume(common::Context &context) const;

        static TLVTag consumeTag(common::Context &context);

        static common::Context &consumeExpectedTag(common::Context &context, common::TLVTag const &expectedTag);

        static std::size_t consumeLength(common::Context &context);

        static std::span<std::uint8_t const> consumeExpectedElement(common::Context &context, common::TLVTag const &expectedTag);
    };
}
