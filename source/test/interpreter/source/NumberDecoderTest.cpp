// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/NumberDecoder.h"

namespace interpreter::detail::common
{
  TEST(consumeUInteger, min1)
  {
    auto context = Context({0xff, 1, 1, 0xff});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeUInteger1(context), 1);
    EXPECT_EQ(NumberDecoder::consumeUInteger1AsString(context), "1");
  }

  TEST(consumeUInteger, max1)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeUInteger1(context), 255);
    EXPECT_EQ(NumberDecoder::consumeUInteger1AsString(context), "255");
  }

  TEST(decodeUInteger, max1)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0x1};
    EXPECT_EQ(NumberDecoder::decodeUInteger1({data.data(), data.size()}), 255);
  }

  TEST(decodeUInteger, empty1)
  {
    auto const data = std::vector<std::uint8_t>{};
    EXPECT_THROW(NumberDecoder::decodeUInteger1({data.data(), data.size()}), std::runtime_error);
  }

  TEST(consumeUInteger, min2)
  {
    auto context = Context({0xff, 0, 1, 0, 1, 0xff});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeUInteger2(context), 1);
    EXPECT_EQ(NumberDecoder::consumeUInteger2AsString(context), "1");
  }

  TEST(consumeUInteger, max2)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeUInteger2(context), 65535);
    EXPECT_EQ(NumberDecoder::consumeUInteger2AsString(context), "65535");
  }

  TEST(decodeUInteger, max2)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0xff, 0x1};
    EXPECT_EQ(NumberDecoder::decodeUInteger2({data.data(), data.size()}), 65535);
  }

  TEST(decodeUInteger, empty2)
  {
    auto const data = std::vector<std::uint8_t>{0};
    EXPECT_THROW(NumberDecoder::decodeUInteger2({data.data(), data.size()}), std::runtime_error);
  }

  TEST(consumeUInteger, min3)
  {
    auto context = Context({0xff, 0, 0, 1, 0, 0, 1, 0xff}); // big endian 1
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeUInteger3(context), 1);
    EXPECT_EQ(NumberDecoder::consumeUInteger3AsString(context), "1");
  }

  TEST(consumeUInteger, max3)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeUInteger3(context), 16777215);
    EXPECT_EQ(NumberDecoder::consumeUInteger3AsString(context), "16777215");
  }

  TEST(decodeUInteger, max3)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0x1};
    EXPECT_EQ(NumberDecoder::decodeUInteger3({data.data(), data.size()}), 16777215);
  }

  TEST(decodeUInteger, empty3)
  {
    auto const data = std::vector<std::uint8_t>{0, 0};
    EXPECT_THROW(NumberDecoder::decodeUInteger3({data.data(), data.size()}), std::runtime_error);
  }

  TEST(consumeUInteger, min4)
  {
    auto context = Context({0xff, 0, 0, 0, 1, 0, 0, 0, 1, 0xff}); // big endian 1
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeUInteger4(context), 1);
    EXPECT_EQ(NumberDecoder::consumeUInteger4AsString(context), "1");
  }

  TEST(consumeUInteger, max4)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeUInteger4(context), 4294967295);
    EXPECT_EQ(NumberDecoder::consumeUInteger4AsString(context), "4294967295");
  }

  TEST(decodeUInteger, max4)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0x1};
    EXPECT_EQ(NumberDecoder::decodeUInteger4({data.data(), data.size()}), 4294967295);
  }

  TEST(decodeUInteger, empty4)
  {
    auto const data = std::vector<std::uint8_t>{0, 0, 0};
    EXPECT_THROW(NumberDecoder::decodeUInteger4({data.data(), data.size()}), std::runtime_error);
  }

  TEST(decodeSInteger, negative1)
  {
    auto data = std::vector<std::uint8_t>{0x81};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -127LL);

    data = std::vector<std::uint8_t>{0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2LL);
  }

  TEST(decodeSInteger, positive1)
  {
    auto data = std::vector<std::uint8_t>{0x7e};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 126LL);

    data = std::vector<std::uint8_t>{0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 1LL);
  }

  TEST(decodeSInteger, negative2)
  {
    auto data = std::vector<std::uint8_t>{0x80, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -32767LL);

    data = std::vector<std::uint8_t>{0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2LL);
  }

  TEST(decodeSInteger, positive2)
  {
    auto data = std::vector<std::uint8_t>{0x7f, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 32766LL);

    data = std::vector<std::uint8_t>{0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 1LL);
  }

  TEST(decodeSInteger, negative3)
  {
    auto data = std::vector<std::uint8_t>{0x80, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -8388607LL);

    data = std::vector<std::uint8_t>{0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2LL);
  }

  TEST(decodeSInteger, positive3)
  {
    auto data = std::vector<std::uint8_t>{0x7f, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 8388606LL);

    data = std::vector<std::uint8_t>{0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 1LL);
  }

  TEST(decodeSInteger, negative4)
  {
    auto data = std::vector<std::uint8_t>{0x80, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2147483647LL);

    data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2LL);
  }

  TEST(decodeSInteger, positive4)
  {
    auto data = std::vector<std::uint8_t>{0x7f, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 2147483646LL);

    data = std::vector<std::uint8_t>{0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 1LL);
  }

  TEST(decodeSInteger, negative5)
  {
    auto data = std::vector<std::uint8_t>{0x80, 0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -549755813887LL);

    data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2LL);
  }

  TEST(decodeSInteger, positive5)
  {
    auto data = std::vector<std::uint8_t>{0x7f, 0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 549755813886LL);

    data = std::vector<std::uint8_t>{0x00, 0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 1LL);
  }

  TEST(decodeSInteger, negative6)
  {
    auto data = std::vector<std::uint8_t>{0x80, 0x00, 0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -140737488355327LL);

    data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2LL);
  }

  TEST(decodeSInteger, positive6)
  {
    auto data = std::vector<std::uint8_t>{0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 140737488355326LL);

    data = std::vector<std::uint8_t>{0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 1LL);
  }

  TEST(decodeSInteger, negative7)
  {
    auto data = std::vector<std::uint8_t>{0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -36028797018963967LL);

    data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2LL);
  }

  TEST(decodeSInteger, positive7)
  {
    auto data = std::vector<std::uint8_t>{0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 36028797018963966LL);

    data = std::vector<std::uint8_t>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 1LL);
  }

  TEST(decodeSInteger, negative8)
  {
    auto data = std::vector<std::uint8_t>{0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -9223372036854775807LL);

    data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), -2LL);
  }

  TEST(decodeSInteger, positive8)
  {
    auto data = std::vector<std::uint8_t>{0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 9223372036854775806LL);

    data = std::vector<std::uint8_t>{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data(), data.size()}), 1LL);
  }

  TEST(decodeSInteger, boundary)
  {
    auto const data = std::vector<std::uint8_t>{0x23, 0x80, 0x00, 0x00, 0x00, 0x01, 0x42};
    EXPECT_EQ(NumberDecoder::decodeSInteger({data.data() + 1, data.size() - 2}), -549755813887LL);
  }

  TEST(decodeSInteger, empty)
  {
    auto const data = std::vector<std::uint8_t>{};
    EXPECT_THROW(NumberDecoder::decodeSInteger({data.data(), data.size()}), std::runtime_error);
  }

  TEST(decodeSInteger, exceedMax)
  {
    auto const data = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    EXPECT_THROW(NumberDecoder::decodeSInteger({data.data(), data.size()}), std::runtime_error);
  }
}
