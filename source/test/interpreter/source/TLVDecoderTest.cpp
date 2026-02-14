// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/TLVDecoder.h"

namespace interpreter::detail::common
{
    TEST(TLVTag, empty)
    {
        auto tag = TLVTag{};
        EXPECT_EQ(0, tag[0]);
        EXPECT_EQ(0, tag[1]);
        EXPECT_EQ(0, tag[2]);
        EXPECT_EQ(0, tag[3]);
    }

    TEST(TLVTag, compareEqual)
    {
        EXPECT_EQ((TLVTag{}), (TLVTag{}));
        EXPECT_EQ((TLVTag{0, 0}), (TLVTag{0, 0}));
        auto tag = TLVTag{};
        tag.assign(2, 5);
        EXPECT_EQ((TLVTag{0, 0, 5}), tag);
    }

    TEST(TLVTag, compareNotEqual)
    {
        EXPECT_NE((TLVTag{}), (TLVTag{1}));
        EXPECT_NE((TLVTag{1}), (TLVTag{}));
        EXPECT_NE((TLVTag{0}), (TLVTag{0, 0}));
        EXPECT_NE((TLVTag{0, 0}), (TLVTag{0, 0, 0}));
        EXPECT_NE((TLVTag{1}), (TLVTag{0, 0, 0, 1}));
    }

    TEST(TLVTag, constSubscription)
    {
        auto tag = TLVTag{23, 42, 5, 6};
        EXPECT_EQ(23, tag[0]);
        EXPECT_EQ(42, tag[1]);
        EXPECT_EQ(5, tag[2]);
        EXPECT_EQ(6, tag[3]);
    }

    TEST(TLVTag, subscription)
    {
        auto tag = TLVTag{};
        tag.assign(0, 23);
        tag.assign(1, 42);
        tag.assign(2, 5);
        tag.assign(3, 6);
        EXPECT_EQ((TLVTag{23, 42, 5, 6}), tag);
    }

    TEST(TLVTag, toHexString)
    {
        EXPECT_EQ((TLVTag{}).toHexString(), "");
        EXPECT_EQ((TLVTag{0, 8}).toHexString(), "0008");
        EXPECT_EQ((TLVTag{0, 0, 0, 1}).toHexString(), "00000001");
        EXPECT_EQ((TLVTag{1, 0, 0, 0}).toHexString(), "01000000");
        EXPECT_EQ((TLVTag{0xff, 0, 0, 0}).toHexString(), "FF000000");
        EXPECT_EQ((TLVTag{23, 42, 80, 255}).toHexString(), "172A50FF");

        auto tag = TLVTag{};
        EXPECT_EQ(tag[0], 0);
        EXPECT_EQ(tag.toHexString(), "");
        tag.assign(0, 0);
        EXPECT_EQ(tag.toHexString(), "00");
        tag.assign(1, 7);
        EXPECT_EQ(tag.toHexString(), "0007");
    }

    TEST(TLVTag, ensureEqual)
    {
        EXPECT_NO_THROW((TLVTag{}.ensureEqual(TLVTag{})));
        EXPECT_NO_THROW((TLVTag{23}.ensureEqual(TLVTag{23})));
        EXPECT_NO_THROW((TLVTag{0, 1}.ensureEqual(TLVTag{0, 1})));
        EXPECT_NO_THROW((TLVTag{1, 2, 3, 4}.ensureEqual(TLVTag{1, 2, 3, 4})));
    }

    TEST(TLVTag, ensureNotEqual)
    {
        EXPECT_THROW((TLVTag{}.ensureEqual(TLVTag{0})), std::runtime_error);
        EXPECT_THROW((TLVTag{23}.ensureEqual(TLVTag{0, 23})), std::runtime_error);
        EXPECT_THROW((TLVTag{23}.ensureEqual(TLVTag{23, 0})), std::runtime_error);
        EXPECT_THROW((TLVTag{0, 1}.ensureEqual(TLVTag{1, 0})), std::runtime_error);
        EXPECT_THROW((TLVTag{1, 2, 3, 4}.ensureEqual(TLVTag{1, 2, 3})), std::runtime_error);
        EXPECT_THROW((TLVTag{1, 2, 3, 4}.ensureEqual(TLVTag{})), std::runtime_error);
    }

    TEST(TLVDecoder, consume4ByteTagStatic)
    {
        auto context = Context(std::vector<std::uint8_t>{0b01111111, 0b10000001, 0b11111111, 0b01010101, 0x23});
        auto const tag = TLVDecoder::consumeTag(context);
        EXPECT_EQ(tag, (TLVTag{0x7f, 0x81, 0xff, 0x55}));
        EXPECT_EQ(1, context.getRemainingSize());
    }

    TEST(TLVDecoder, consume3ByteTagStatic)
    {
        auto context = Context(std::vector<std::uint8_t>{0b01111111, 0b10000001, 0b01111111, 0x23});
        auto const tag = TLVDecoder::consumeTag(context);
        EXPECT_EQ(tag, (TLVTag{0x7f, 0x81, 0x7f}));
        EXPECT_EQ(1, context.getRemainingSize());
    }

    TEST(TLVDecoder, consume2ByteTagStatic)
    {
        auto context = Context(std::vector<std::uint8_t>{0b01111111, 0b00100001, 0x42});
        auto const tag = TLVDecoder::consumeTag(context);
        EXPECT_EQ(tag, (TLVTag{0x7f, 0x21}));
        EXPECT_EQ(1, context.getRemainingSize());
    }

    TEST(TLVDecoder, consume1ByteTagStatic)
    {
        auto context = Context(std::vector<std::uint8_t>{0b10011010, 0x42});
        auto const tag = TLVDecoder::consumeTag(context);
        EXPECT_EQ(tag, (TLVTag{0x9a}));
        EXPECT_EQ(1, context.getRemainingSize());
    }
}
