// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

namespace interpreter::detail::common
{
  TEST(consumeString, readAndStopAtNull)
  {
    auto const source = std::vector<std::uint8_t>{'R', 'P', 'E', 'X', '4', 'F', '-', '4', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto context = Context(source, "");
    EXPECT_EQ(consumeString(context, 20), std::string("RPEX4F-4"));
  }

  TEST(consumeString, readAndTrimTrailingSpaces)
  {
    auto const source = std::vector<std::uint8_t>{'A', 'B', 'C', ' ', '\n', ' ', ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto context = Context(source, "");
    EXPECT_EQ(consumeString(context, 20), std::string("ABC"));
  }

  TEST(consumeString, readAll)
  {
    auto const source = std::vector<std::uint8_t>{'R', 'P', 'E', 'X', '4', 'F', '-', '4'};
    auto context = Context(source, "");
    EXPECT_EQ(consumeString(context, 8), std::string("RPEX4F-4"));
  }

  TEST(consumeString, readEmpty)
  {
    auto const source = std::vector<std::uint8_t>{0};
    auto context = Context(source, "");
    EXPECT_EQ(consumeString(context, 8), std::string(""));
  }

  TEST(getNumeric, min8)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 1, 0xff};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(consumeInteger1(context), 1);
  }

  TEST(getNumeric, max8)
  {
    auto const source = std::vector<std::uint8_t>{0xfe, 0xff, 0xfe};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(consumeInteger1(context), 255);
  }

  TEST(getNumeric, min16)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 1, 0xff};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(consumeInteger2(context), 1);
  }

  TEST(getNumeric, max16)
  {
    auto const source = std::vector<std::uint8_t>{0xfe, 0xff, 0xff, 0xfe};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(consumeInteger2(context), 65535);
  }

  TEST(getNumeric, min24)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 0, 1, 0xff}; // big endian 1
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(consumeInteger3(context), 1);
  }

  TEST(getNumeric, max24)
  {
    auto const source = std::vector<std::uint8_t>{0xfe, 0xff, 0xff, 0xff, 0xfe};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(consumeInteger3(context), 16777215);
  }

  TEST(getNumeric, min32)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 0, 0, 1, 0xff}; // big endian 1
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(consumeInteger4(context), 1);
  }

  TEST(getNumeric, max32)
  {
    auto const source = std::vector<std::uint8_t>{0xfe, 0xff, 0xff, 0xff, 0xff, 0xfe};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(consumeInteger4(context), 4294967295);
  }

  TEST(consumeDateTimeCompact4, initial)
  {
    auto const source = std::vector<std::uint8_t>{0x28, 0x39, 0x70, 0x62};
    auto context = Context(source, "");
    EXPECT_EQ(consumeDateTimeCompact4(context), "2010-01-25T14:03:02");
  }

  TEST(consumeDateTime3, initial)
  {
    auto const source = std::vector<std::uint8_t>{'2', '7', '1', '0', '2', '0', '2', '0', '1', '3', '4', '5'};
    auto context = Context(source, "");
    EXPECT_EQ(consumeDateTime3(context), "2020-10-27T13:45:00");
  }

  TEST(consumeDate8, initial)
  {
    auto const source = std::vector<std::uint8_t>{'1', '3', '0', '1', '2', '0', '2', '1'};
    auto context = Context(source, "");
    EXPECT_EQ(consumeDate8(context), "2021-01-13");
  }

  TEST(bytesToHexString, filled)
  {
    auto const source = std::vector<std::uint8_t>{0x12, 0x0A, 0xAB, 0x00, 0xFF};
    EXPECT_EQ(bytesToHexString(source), "120AAB00FF");
  }

  TEST(bytesToHexString, empty)
  {
    auto const source = std::vector<std::uint8_t>{};
    EXPECT_EQ(bytesToHexString(source), "");
  }

  TEST(bytesToHexString, array)
  {
    EXPECT_EQ(bytesToHexString(std::array<std::uint8_t, 3>{0x12, 0x0A, 0xAB}), "120AAB");
    EXPECT_EQ(bytesToHexString(std::array<std::uint8_t, 3>{0x12, 0x0A}), "120A00");
    EXPECT_EQ(bytesToHexString(std::array<std::uint8_t, 3>{}), "000000");
    EXPECT_EQ(bytesToHexString(std::array<std::uint8_t, 5>{0x11, 0x22, 0x33, 0x44, 0x55}), "1122334455");
  }

  TEST(bytesToHexString, integer4)
  {
    EXPECT_EQ(bytesToHexString(std::uint32_t(0)), "00000000");
    EXPECT_EQ(bytesToHexString(std::uint32_t(1)), "00000001");
    EXPECT_EQ(bytesToHexString(std::uint32_t(0xffffffff)), "FFFFFFFF");
  }

  TEST(bytesToHexString, integer2)
  {
    EXPECT_EQ(bytesToHexString(std::uint16_t()), "0000");
    EXPECT_EQ(bytesToHexString(std::uint16_t(1)), "0001");
    EXPECT_EQ(bytesToHexString(std::uint16_t(0xffff)), "FFFF");
  }

  TEST(bytesToHexString, integer1)
  {
    EXPECT_EQ(bytesToHexString(std::uint8_t()), "00");
    EXPECT_EQ(bytesToHexString(std::uint8_t(1)), "01");
    EXPECT_EQ(bytesToHexString(std::uint8_t(0xff)), "FF");
  }
}
