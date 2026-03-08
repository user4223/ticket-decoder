// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/NumberDecoder.h"

namespace interpreter::detail::common
{
  TEST(consumeNumeric, min1)
  {
    auto context = Context({0xff, 1, 1, 0xff});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger1(context), 1);
    EXPECT_EQ(NumberDecoder::consumeInteger1AsString(context), "1");
  }

  TEST(consumeNumeric, max1)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger1(context), 255);
    EXPECT_EQ(NumberDecoder::consumeInteger1AsString(context), "255");
  }

  TEST(decodeNumeric, max1)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0x1};
    EXPECT_EQ(NumberDecoder::decodeInteger1({data.data(), data.size()}), 255);
  }

  TEST(decodeNumeric, empty1)
  {
    auto const data = std::vector<std::uint8_t>{};
    EXPECT_THROW(NumberDecoder::decodeInteger1({data.data(), data.size()}), std::runtime_error);
  }

  TEST(consumeNumeric, min2)
  {
    auto context = Context({0xff, 0, 1, 0, 1, 0xff});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger2(context), 1);
    EXPECT_EQ(NumberDecoder::consumeInteger2AsString(context), "1");
  }

  TEST(consumeNumeric, max2)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger2(context), 65535);
    EXPECT_EQ(NumberDecoder::consumeInteger2AsString(context), "65535");
  }

  TEST(decodeNumeric, max2)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0xff, 0x1};
    EXPECT_EQ(NumberDecoder::decodeInteger2({data.data(), data.size()}), 65535);
  }

  TEST(decodeNumeric, empty2)
  {
    auto const data = std::vector<std::uint8_t>{0};
    EXPECT_THROW(NumberDecoder::decodeInteger2({data.data(), data.size()}), std::runtime_error);
  }

  TEST(consumeNumeric, min3)
  {
    auto context = Context({0xff, 0, 0, 1, 0, 0, 1, 0xff}); // big endian 1
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger3(context), 1);
    EXPECT_EQ(NumberDecoder::consumeInteger3AsString(context), "1");
  }

  TEST(consumeNumeric, max3)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger3(context), 16777215);
    EXPECT_EQ(NumberDecoder::consumeInteger3AsString(context), "16777215");
  }

  TEST(decodeNumeric, max3)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0x1};
    EXPECT_EQ(NumberDecoder::decodeInteger3({data.data(), data.size()}), 16777215);
  }

  TEST(decodeNumeric, empty3)
  {
    auto const data = std::vector<std::uint8_t>{0, 0};
    EXPECT_THROW(NumberDecoder::decodeInteger3({data.data(), data.size()}), std::runtime_error);
  }

  TEST(consumeNumeric, min4)
  {
    auto context = Context({0xff, 0, 0, 0, 1, 0, 0, 0, 1, 0xff}); // big endian 1
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger4(context), 1);
    EXPECT_EQ(NumberDecoder::consumeInteger4AsString(context), "1");
  }

  TEST(consumeNumeric, max4)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger4(context), 4294967295);
    EXPECT_EQ(NumberDecoder::consumeInteger4AsString(context), "4294967295");
  }

  TEST(decodeNumeric, max4)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0x1};
    EXPECT_EQ(NumberDecoder::decodeInteger4({data.data(), data.size()}), 4294967295);
  }

  TEST(decodeNumeric, empty4)
  {
    auto const data = std::vector<std::uint8_t>{0, 0, 0};
    EXPECT_THROW(NumberDecoder::decodeInteger4({data.data(), data.size()}), std::runtime_error);
  }
}
