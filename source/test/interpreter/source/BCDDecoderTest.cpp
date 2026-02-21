// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/BCDDecoder.h"
#include "lib/interpreter/detail/common/include/Context.h"

namespace interpreter::detail::common
{
    auto const buffer = std::vector<std::uint8_t>{0x20, 0x26, 0x0, 0x0, 0x99, 0x99};

    TEST(BCDDecoder, decodePackedInteger1)
    {
        EXPECT_EQ(20, BCDDecoder::decodePackedInteger1(buffer[0]));
        EXPECT_EQ(20, BCDDecoder::decodePackedInteger1(buffer[0]));
        EXPECT_EQ(26, BCDDecoder::decodePackedInteger1(buffer[1]));
        EXPECT_EQ(26, BCDDecoder::decodePackedInteger1(buffer[1]));
    }

    TEST(BCDDecoder, decodePackedInteger1Min)
    {
        EXPECT_EQ(0, BCDDecoder::decodePackedInteger1(buffer[2]));
        EXPECT_EQ(0, BCDDecoder::decodePackedInteger1(buffer[3]));
    }

    TEST(BCDDecoder, decodePackedInteger1Max)
    {
        EXPECT_EQ(99, BCDDecoder::decodePackedInteger1(buffer[4]));
        EXPECT_EQ(99, BCDDecoder::decodePackedInteger1(buffer[5]));
    }

    TEST(BCDDecoder, consumePackedInteger1)
    {
        auto context = common::Context({0x20, 0x26});
        EXPECT_EQ(20, BCDDecoder::consumePackedInteger1(context));
        EXPECT_FALSE(context.isEmpty());
        EXPECT_EQ(26, BCDDecoder::consumePackedInteger1(context));
        EXPECT_TRUE(context.isEmpty());
    }

    TEST(BCDDecoder, decodePackedInteger2)
    {
        EXPECT_EQ(2026, BCDDecoder::decodePackedInteger2({buffer.begin(), 2}));
        EXPECT_EQ(2026, BCDDecoder::decodePackedInteger2({buffer.begin(), buffer.end()}));
        EXPECT_EQ(2600, BCDDecoder::decodePackedInteger2({buffer.begin() + 1, 2}));
        EXPECT_EQ(2600, BCDDecoder::decodePackedInteger2({buffer.begin() + 1, buffer.end()}));
    }

    TEST(BCDDecoder, decodePackedInteger2Min)
    {
        EXPECT_EQ(0, BCDDecoder::decodePackedInteger2({buffer.begin() + 2, 2}));
        EXPECT_EQ(0, BCDDecoder::decodePackedInteger2({buffer.begin() + 2, buffer.end()}));
    }

    TEST(BCDDecoder, decodePackedInteger2Max)
    {
        EXPECT_EQ(9999, BCDDecoder::decodePackedInteger2({buffer.begin() + 4, 2}));
        EXPECT_EQ(9999, BCDDecoder::decodePackedInteger2({buffer.begin() + 4, buffer.end()}));
    }

    TEST(BCDDecoder, decodePackedInteger2Invalid)
    {
        EXPECT_THROW(BCDDecoder::decodePackedInteger2({buffer.begin(), 1}), std::runtime_error);
    }

    TEST(BCDDecoder, consumePackedInteger2)
    {
        auto context = common::Context({0x20, 0x26});
        EXPECT_EQ(2026, BCDDecoder::consumePackedInteger2(context));
        EXPECT_TRUE(context.isEmpty());
    }
}
