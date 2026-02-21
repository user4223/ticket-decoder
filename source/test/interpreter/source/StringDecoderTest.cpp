// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/StringDecoder.h"

namespace interpreter::detail::common
{
  TEST(StringDecoder, consumeUtf8)
  {
    auto const buffer = std::string("öäü");
    auto context = Context(std::span<std::uint8_t const>{(std::uint8_t const *)buffer.data(), 6});
    EXPECT_EQ(StringDecoder::consumeString(context, 6), std::string("öäü"));

    context = Context({0xC3, 0xBC, 0xC3, 0xB6, 0xC3, 0xA4, 0xC3, 0x9F, 0xC3, 0x9C, 0xC3, 0x96, 0xC3, 0x84});
    EXPECT_EQ(StringDecoder::consumeString(context, 6), std::string("üöä"));
    EXPECT_EQ(StringDecoder::consumeString(context, 8), std::string("ßÜÖÄ"));
  }

  TEST(StringDecoder, consumeAndStopAtNull)
  {
    auto context = Context({'R', 'P', 'E', 'X', '4', 'F', '-', '4', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    EXPECT_EQ(StringDecoder::consumeString(context, 20), std::string("RPEX4F-4"));
  }

  TEST(StringDecoder, consumeAndTrimTrailingSpaces)
  {
    auto context = Context({'A', 'B', 'C', ' ', '\n', ' ', ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    EXPECT_EQ(StringDecoder::consumeString(context, 20), std::string("ABC"));
  }

  TEST(StringDecoder, consumeAll)
  {
    auto context = Context({'R', 'P', 'E', 'X', '4', 'F', '-', '4'});
    EXPECT_EQ(StringDecoder::consumeString(context, 8), std::string("RPEX4F-4"));
  }

  TEST(StringDecoder, consumeEmpty)
  {
    auto context = Context({0});
    EXPECT_EQ(StringDecoder::consumeString(context, 8), std::string(""));
  }

  TEST(bytesToHexString, filled)
  {
    auto const source = std::vector<std::uint8_t>{0x12, 0x0A, 0xAB, 0x00, 0xFF};
    EXPECT_EQ(StringDecoder::bytesToHexString(source), "120AAB00FF");
  }

  TEST(bytesToHexString, empty)
  {
    auto const source = std::vector<std::uint8_t>{};
    EXPECT_EQ(StringDecoder::bytesToHexString(source), "");
  }

  TEST(bytesToHexString, array)
  {
    EXPECT_EQ(StringDecoder::bytesToHexString(std::array<std::uint8_t, 1>{}), "00");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::array<std::uint8_t, 3>{0x12, 0x0A, 0xAB}), "120AAB");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::array<std::uint8_t, 3>{0x12, 0x0A}), "120A00");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::array<std::uint8_t, 3>{}), "000000");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::array<std::uint8_t, 5>{0x11, 0x22, 0x33, 0x44, 0x55}), "1122334455");
  }

  TEST(bytesToHexString, integer4)
  {
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint32_t(0)), "00000000");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint32_t(1)), "00000001");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint32_t(0xffffffff)), "FFFFFFFF");
  }

  TEST(bytesToHexString, integer2)
  {
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint16_t()), "0000");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint16_t(1)), "0001");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint16_t(0xffff)), "FFFF");
  }

  TEST(bytesToHexString, integer1)
  {
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint8_t()), "00");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint8_t(1)), "01");
    EXPECT_EQ(StringDecoder::bytesToHexString(std::uint8_t(0xff)), "FF");
  }
}
