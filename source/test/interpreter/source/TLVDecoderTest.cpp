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
        EXPECT_EQ(0, tag.size());
        tag.assign(0, 23);
        EXPECT_EQ(1, tag.size());
        tag.assign(1, 42);
        EXPECT_EQ(2, tag.size());
        tag.assign(2, 5);
        EXPECT_EQ(3, tag.size());
        tag.assign(3, 6);
        EXPECT_EQ((TLVTag{23, 42, 5, 6}), tag);
    }

    TEST(TLVTag, size)
    {
        auto tag = TLVTag{};
        tag.assign(2, 23);
        EXPECT_EQ(3, tag.size());
        tag.assign(1, 42);
        EXPECT_EQ(3, tag.size());
        tag.assign(0, 5);
        EXPECT_EQ(3, tag.size());
        tag.assign(3, 6);
        EXPECT_EQ(4, tag.size());
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

    TEST(TLVDecoder, consumeExpectedTagStatic)
    {
        auto context = Context(std::vector<std::uint8_t>{0b01111111, 0b00100001, 0x42});
        EXPECT_NO_THROW(TLVDecoder::consumeExpectedTag(context, {0x7f, 0x21}));
        EXPECT_EQ(1, context.getRemainingSize());
        EXPECT_NO_THROW(TLVDecoder::consumeExpectedTag(context, {0x42}));
        EXPECT_EQ(0, context.getRemainingSize());
    }

    TEST(TLVDecoder, consumeNotExpectedTagStatic)
    {
        auto context = Context(std::vector<std::uint8_t>{0b01111111, 0b00100001, 0x42});
        EXPECT_THROW(TLVDecoder::consumeExpectedTag(context, {0x7e, 0x21}), std::runtime_error);
        EXPECT_EQ(1, context.getRemainingSize());
        EXPECT_THROW(TLVDecoder::consumeExpectedTag(context, {0x41}), std::runtime_error);
        EXPECT_EQ(0, context.getRemainingSize());
    }

    TEST(TLVDecoder, consume1ByteLength)
    {
        auto context = Context(std::vector<std::uint8_t>{1, 127, 0x80});
        EXPECT_EQ(1, TLVDecoder::consumeLength(context));
        EXPECT_EQ(127, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0, TLVDecoder::consumeLength(context));
    }

    TEST(TLVDecoder, consume2ByteLength)
    {
        auto context = Context(std::vector<std::uint8_t>{0x81, 23, 0x81, 0, 0x81, 0xff});
        EXPECT_EQ(23, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0, TLVDecoder::consumeLength(context));
        EXPECT_EQ(255, TLVDecoder::consumeLength(context));
    }

    TEST(TLVDecoder, consume3ByteLength)
    {
        auto context = Context(std::vector<std::uint8_t>{0x82, 1, 2, 0x82, 0, 0, 0x82, 0xff, 0xff});
        EXPECT_EQ(0x0102, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0, TLVDecoder::consumeLength(context));
        EXPECT_EQ(std::numeric_limits<std::uint16_t>::max(), TLVDecoder::consumeLength(context));
    }

    TEST(TLVDecoder, consume4ByteLength)
    {
        auto context = Context(std::vector<std::uint8_t>{0x83, 1, 2, 3, 0x83, 0, 0, 0, 0x83, 0xff, 0xff, 0xff});
        EXPECT_EQ(0x010203, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0xffffff, TLVDecoder::consumeLength(context));
    }

    TEST(TLVDecoder, consume5ByteLength)
    {
        auto context = Context(std::vector<std::uint8_t>{0x84, 1, 2, 3, 4, 0x84, 0, 0, 0, 0, 0x84, 0xff, 0xff, 0xff, 0xff});
        EXPECT_EQ(0x01020304, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0xffffffff, TLVDecoder::consumeLength(context));
    }

    TEST(TLVDecoder, consume6ByteLength)
    {
        auto context = Context(std::vector<std::uint8_t>{0x85, 1, 2, 3, 4, 5, 0x85, 0, 0, 0, 0, 0, 0x85, 0xff, 0xff, 0xff, 0xff, 0xff});
        EXPECT_EQ(0x0102030405, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0xffffffffff, TLVDecoder::consumeLength(context));
    }

    TEST(TLVDecoder, consume9ByteLength)
    {
        auto context = Context(std::vector<std::uint8_t>{0x88, 1, 2, 3, 4, 5, 6, 7, 8, 0x88, 0, 0, 0, 0, 0, 0, 0, 0, 0x88, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff});
        EXPECT_EQ(0x0102030405060708, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0, TLVDecoder::consumeLength(context));
        EXPECT_EQ(0xffffffffffffffff, TLVDecoder::consumeLength(context));
    }

    TEST(TLVDecoder, consume10ByteLength)
    {
        auto context = Context(std::vector<std::uint8_t>{0x89, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        EXPECT_THROW(TLVDecoder::consumeLength(context), std::runtime_error);
    }

    TEST(TLVDecoder, consumeExpectedElement)
    {
        auto context = Context(std::vector<std::uint8_t>{0x9a, 0x81, 0x02, 0x23, 0x42});
        auto const value = TLVDecoder::consumeExpectedElement(context, {0x9a});
        EXPECT_EQ(value.size(), 2);
        EXPECT_EQ(0x23, value[0]);
        EXPECT_EQ(0x42, value[1]);
    }

    TEST(TLVDecoder, consumeUnexpectedElement)
    {
        auto context = Context(std::vector<std::uint8_t>{0x9a, 0x81, 0x02, 0x23, 0x42});
        EXPECT_THROW(TLVDecoder::consumeExpectedElement(context, {0x9b}), std::runtime_error);
    }
}
