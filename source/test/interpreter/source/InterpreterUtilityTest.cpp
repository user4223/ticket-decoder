// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

namespace interpreter::detail::common
{
  TEST(getAlphanumeric, readAndStopAtNull)
  {
    auto const source = std::vector<std::uint8_t>{'R', 'P', 'E', 'X', '4', 'F', '-', '4', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto position = source.begin();
    EXPECT_EQ(getAlphanumeric(position, 20), std::string("RPEX4F-4"));
  }

  TEST(getAlphanumeric, readAndTrimTrailingSpaces)
  {
    auto const source = std::vector<std::uint8_t>{'A', 'B', 'C', ' ', '\n', ' ', ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    auto position = source.begin();
    EXPECT_EQ(getAlphanumeric(position, 20), std::string("ABC"));
  }

  TEST(getAlphanumeric, readAll)
  {
    auto const source = std::vector<std::uint8_t>{'R', 'P', 'E', 'X', '4', 'F', '-', '4'};
    auto position = source.begin();
    EXPECT_EQ(getAlphanumeric(position, 8), std::string("RPEX4F-4"));
  }

  TEST(getAlphanumeric, readEmpty)
  {
    auto const source = std::vector<std::uint8_t>{0};
    auto position = source.begin();
    EXPECT_EQ(getAlphanumeric(position, 8), std::string(""));
  }

  TEST(getNumeric, min8)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 1, 0xff};
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric8(position), 1);
  }

  TEST(getNumeric, max8)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0xff, 0xff};
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric8(position), 255);
  }

  TEST(getNumeric, min16)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 1, 0xff};
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric16(position), 1);
  }

  TEST(getNumeric, max16)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff};
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric16(position), 65535);
  }

  TEST(getNumeric, min24)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 0, 1, 0xff}; // big endian 1
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric24(position), 1);
  }

  TEST(getNumeric, max24)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0xff};
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric24(position), 16777215);
  }

  TEST(getNumeric, min32)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 0, 0, 1, 0xff}; // big endian 1
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric32(position), 1);
  }

  TEST(getNumeric, max32)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric32(position), 4294967295);
  }

  TEST(getDateTimeCompact, initial)
  {
    auto const source = std::vector<std::uint8_t>{0x28, 0x39, 0x70, 0x62};
    auto position = source.begin();
    EXPECT_EQ(getDateTimeCompact(position), "2010-01-25T14:03:02");
  }

  TEST(getDateTime12, initial)
  {
    auto const source = std::vector<std::uint8_t>{'2', '7', '1', '0', '2', '0', '2', '0', '1', '3', '4', '5'};
    auto position = source.begin();
    EXPECT_EQ(getDateTime12(position), "2020-10-27T13:45:00");
  }

  TEST(getDate8, initial)
  {
    auto const source = std::vector<std::uint8_t>{'1', '3', '0', '1', '2', '0', '2', '1'};
    auto position = source.begin();
    EXPECT_EQ(getDate8(position), "2021-01-13");
  }
}
