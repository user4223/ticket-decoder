// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

namespace interpreter::detail::common
{
  TEST(getAlphanumeric, readAndStopAtNull)
  {
    auto const source = std::vector<std::uint8_t>{'R', 'P', 'E', 'X', '4', 'F', '-', '4', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto context = Context(source, "");
    EXPECT_EQ(getAlphanumeric(context, 20), std::string("RPEX4F-4"));
  }

  TEST(getAlphanumeric, readAndTrimTrailingSpaces)
  {
    auto const source = std::vector<std::uint8_t>{'A', 'B', 'C', ' ', '\n', ' ', ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto context = Context(source, "");
    EXPECT_EQ(getAlphanumeric(context, 20), std::string("ABC"));
  }

  TEST(getAlphanumeric, readAll)
  {
    auto const source = std::vector<std::uint8_t>{'R', 'P', 'E', 'X', '4', 'F', '-', '4'};
    auto context = Context(source, "");
    EXPECT_EQ(getAlphanumeric(context, 8), std::string("RPEX4F-4"));
  }

  TEST(getAlphanumeric, readEmpty)
  {
    auto const source = std::vector<std::uint8_t>{0};
    auto context = Context(source, "");
    EXPECT_EQ(getAlphanumeric(context, 8), std::string(""));
  }

  TEST(getNumeric, min8)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 1, 0xff};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(getNumeric8(context), 1);
  }

  TEST(getNumeric, max8)
  {
    auto const source = std::vector<std::uint8_t>{0xfe, 0xff, 0xfe};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(getNumeric8(context), 255);
  }

  TEST(getNumeric, min16)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 1, 0xff};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(getNumeric16(context), 1);
  }

  TEST(getNumeric, max16)
  {
    auto const source = std::vector<std::uint8_t>{0xfe, 0xff, 0xff, 0xfe};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(getNumeric16(context), 65535);
  }

  TEST(getNumeric, min24)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 0, 1, 0xff}; // big endian 1
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(getNumeric24(context), 1);
  }

  TEST(getNumeric, max24)
  {
    auto const source = std::vector<std::uint8_t>{0xfe, 0xff, 0xff, 0xff, 0xfe};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(getNumeric24(context), 16777215);
  }

  TEST(getNumeric, min32)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 0, 0, 1, 0xff}; // big endian 1
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(getNumeric32(context), 1);
  }

  TEST(getNumeric, max32)
  {
    auto const source = std::vector<std::uint8_t>{0xfe, 0xff, 0xff, 0xff, 0xff, 0xfe};
    auto context = Context(source, "");
    context.ignoreBytes(1);
    EXPECT_EQ(getNumeric32(context), 4294967295);
  }

  TEST(getDateTimeCompact, initial)
  {
    auto const source = std::vector<std::uint8_t>{0x28, 0x39, 0x70, 0x62};
    auto context = Context(source, "");
    EXPECT_EQ(getDateTimeCompact(context), "2010-01-25T14:03:02");
  }

  TEST(getDateTime12, initial)
  {
    auto const source = std::vector<std::uint8_t>{'2', '7', '1', '0', '2', '0', '2', '0', '1', '3', '4', '5'};
    auto context = Context(source, "");
    EXPECT_EQ(getDateTime12(context), "2020-10-27T13:45:00");
  }

  TEST(getDate8, initial)
  {
    auto const source = std::vector<std::uint8_t>{'1', '3', '0', '1', '2', '0', '2', '1'};
    auto context = Context(source, "");
    EXPECT_EQ(getDate8(context), "2021-01-13");
  }

  TEST(bytesToString, filled)
  {
    auto const source = std::vector<std::uint8_t>{0x12, 0x0A, 0xAB, 0x00, 0xFF};
    EXPECT_EQ(bytesToString(source), "0x120AAB00FF");
  }

  TEST(bytesToString, empty)
  {
    auto const source = std::vector<std::uint8_t>{};
    EXPECT_EQ(bytesToString(source), "");
  }
}
