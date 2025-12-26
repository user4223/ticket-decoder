// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/interpreter/detail/common/include/Context.h"

namespace interpreter::detail::common
{
    static std::vector<std::uint8_t> data{0x1, 0x2, 0x3, 0x4, 0x5};

    std::vector<std::uint8_t> toVector(std::span<std::uint8_t const> input)
    {
        return std::vector<std::uint8_t>(input.begin(), input.end());
    }

    TEST(InterpreterContext, peekBytes)
    {
        auto context = Context(data, "origin");
        EXPECT_EQ((std::vector<std::uint8_t>{0x1, 0x2, 0x3}), toVector(context.peekBytes(3)));
        EXPECT_EQ((std::vector<std::uint8_t>{0x1, 0x2, 0x3}), toVector(context.peekBytes(3)));
        EXPECT_EQ(context.position, context.begin);
        context.position += 1;
        EXPECT_EQ((std::vector<std::uint8_t>{0x2, 0x3, 0x4}), toVector(context.peekBytes(3)));
        EXPECT_EQ((std::vector<std::uint8_t>{0x2, 0x3, 0x4, 0x5}), toVector(context.peekBytes(4)));
        EXPECT_EQ(context.position, context.begin + 1);
    }

    TEST(InterpreterContext, peekExceedingBytes)
    {
        auto context = Context(data, "origin");
        EXPECT_EQ(data, toVector(context.peekBytes(5)));
        EXPECT_THROW(context.peekBytes(6), std::runtime_error);
        context.position += 1;
        EXPECT_EQ((std::vector<std::uint8_t>{0x2, 0x3, 0x4, 0x5}), toVector(context.peekBytes(4)));
        EXPECT_THROW(context.peekBytes(5), std::runtime_error);
    }

    TEST(InterpreterContext, consumeBytes)
    {
        auto context = Context(data, "origin");
        EXPECT_EQ((std::vector<std::uint8_t>{}), context.consumeBytes(0));
        EXPECT_EQ((std::vector<std::uint8_t>{0x1}), context.consumeBytes(1));
        EXPECT_EQ((std::vector<std::uint8_t>{0x2, 0x3}), context.consumeBytes(2));
        EXPECT_EQ(context.position, context.begin + 3);
        context.position += 1;
        EXPECT_EQ((std::vector<std::uint8_t>{0x5}), context.consumeBytes(1));
        EXPECT_EQ(context.position, context.begin + 5);
    }

    TEST(InterpreterContext, consumeExceedingBytes)
    {
        auto context = Context(data, "origin");
        EXPECT_EQ(data, context.consumeBytes(5));
        EXPECT_THROW(context.consumeBytes(6), std::runtime_error);
        EXPECT_EQ((std::vector<std::uint8_t>{}), context.consumeBytes(0));
        EXPECT_THROW(context.consumeBytes(1), std::runtime_error);
    }

    TEST(InterpreterContext, consumeMaximalBytes)
    {
        auto context = Context(data, "origin");
        EXPECT_EQ((std::vector<std::uint8_t>{}), context.consumeMaximalBytes(0));
        EXPECT_EQ((std::vector<std::uint8_t>{0x1}), context.consumeMaximalBytes(1));
        EXPECT_EQ((std::vector<std::uint8_t>{0x2, 0x3}), context.consumeMaximalBytes(2));
        EXPECT_EQ((std::vector<std::uint8_t>{0x4, 0x5}), context.consumeMaximalBytes(5));
        EXPECT_EQ((std::vector<std::uint8_t>{}), context.consumeMaximalBytes(5));
    }

    TEST(InterpreterContext, ignoreBytes)
    {
        auto context = Context(data, "origin");
        EXPECT_EQ(0, context.ignoreBytes(0));
        EXPECT_EQ(1, context.ignoreBytes(1));
        EXPECT_EQ(2, context.ignoreBytes(2));
        EXPECT_EQ(context.position, context.begin + 3);
        context.position += 1;
        EXPECT_EQ(1, context.ignoreBytes(1));
        EXPECT_EQ(context.position, context.begin + 5);
    }

    TEST(InterpreterContext, ignoreExceedingBytes)
    {
        auto context = Context(data, "origin");
        EXPECT_EQ(5, context.ignoreBytes(5));
        EXPECT_THROW(context.ignoreBytes(6), std::runtime_error);
        EXPECT_EQ(0, context.ignoreBytes(0));
        EXPECT_THROW(context.ignoreBytes(1), std::runtime_error);
    }
}
