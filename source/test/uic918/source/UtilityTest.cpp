
#include <gtest/gtest.h>

#include "lib/uic918/detail/include/Utility.h"
#include "lib/uic918/u_flex/include/Utility.h"

namespace uic918::detail::utility
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

namespace uic918::u_flex::utility
{
  TEST(daysAndYearToIsoDate, pointerType)
  {
    auto year = 2025l;
    auto day = 100l;
    EXPECT_FALSE(daysAndYearToIsoDate(nullptr, nullptr));
    EXPECT_FALSE(daysAndYearToIsoDate(nullptr, &day));
    EXPECT_FALSE(daysAndYearToIsoDate(&year, nullptr));
    EXPECT_TRUE(daysAndYearToIsoDate(&year, &day));
  }

  TEST(daysAndYearToIsoDate, noLeapYear)
  {
    EXPECT_EQ(daysAndYearToIsoDate(2022l, 1l), "2022-01-01");
    EXPECT_EQ(daysAndYearToIsoDate(2022l, 365l), "2022-12-31");
    EXPECT_EQ(daysAndYearToIsoDate(2022l, 59l), "2022-02-28");
    EXPECT_EQ(daysAndYearToIsoDate(2022l, 60l), "2022-03-01");
    EXPECT_EQ(daysAndYearToIsoDate(2022l, 205l), "2022-07-24");
  }

  TEST(daysAndYearToIsoDate, leapYear)
  {
    EXPECT_EQ(daysAndYearToIsoDate(2020l, 1l), "2020-01-01");
    EXPECT_EQ(daysAndYearToIsoDate(2020l, 366l), "2020-12-31");
    EXPECT_EQ(daysAndYearToIsoDate(2020l, 60l), "2020-02-29");
    EXPECT_EQ(daysAndYearToIsoDate(2020l, 61l), "2020-03-01");
    EXPECT_EQ(daysAndYearToIsoDate(2020l, 316), "2020-11-11");
  }

  TEST(daysAndYearToIsoDate, dayOverflow)
  {
    EXPECT_EQ(daysAndYearToIsoDate(2024l, 366l), "2024-12-31");
    EXPECT_EQ(daysAndYearToIsoDate(2024l, 367l), "2025-01-01");
    EXPECT_EQ(daysAndYearToIsoDate(2024l, 1023l), "2026-0x-0x");
  }

  TEST(daysAndYearToIsoDate, dayUnderflow)
  {
    EXPECT_FALSE(daysAndYearToIsoDate(2024l, 0l));
    EXPECT_FALSE(daysAndYearToIsoDate(2024l, -1l));
  }

  TEST(minutesToIsoTime, pointerType)
  {
    EXPECT_FALSE(minutesToIsoTime(nullptr));
    auto value = 23l;
    EXPECT_EQ("00:23:00", *minutesToIsoTime(&value));
  }

  TEST(minutesToIsoTime, longType)
  {
    EXPECT_EQ("00:00:00", minutesToIsoTime(0l));
    EXPECT_EQ("00:01:00", minutesToIsoTime(1l));
    EXPECT_EQ("00:59:00", minutesToIsoTime(59l));
    EXPECT_EQ("01:00:00", minutesToIsoTime(60l));
    EXPECT_EQ("23:59:00", minutesToIsoTime(24 * 60 - 1l));
  }

  TEST(minutesToIsoTime, negativeMinutes)
  {
    EXPECT_EQ("00:00:00", minutesToIsoTime(-1l));
    EXPECT_EQ("00:00:00", minutesToIsoTime(-1000l));
  }

  TEST(minutesToIsoTime, greaterThanADay)
  {
    EXPECT_EQ("00:00:00", minutesToIsoTime(24 * 60l));
    EXPECT_EQ("00:00:00", minutesToIsoTime(10 * 24 * 60l));
    EXPECT_EQ("23:59:00", minutesToIsoTime(100 * 24 * 60 - 1l));
  }

  TEST(quaterHoursToZoneOffset, valid)
  {
    auto value = -4l;
    EXPECT_EQ("-01:00", *quaterHoursToZoneOffset(&value));
    value = 4;
    EXPECT_EQ("+01:00", *quaterHoursToZoneOffset(&value));
    value = 3;
    EXPECT_EQ("+00:45", *quaterHoursToZoneOffset(&value));
    value = 1;
    EXPECT_EQ("+00:15", *quaterHoursToZoneOffset(&value));
    value = 0;
    EXPECT_EQ("+00:00", *quaterHoursToZoneOffset(&value));
    value = -8;
    EXPECT_EQ("-02:00", *quaterHoursToZoneOffset(&value));
    value = -48;
    EXPECT_EQ("-12:00", *quaterHoursToZoneOffset(&value));
    value = 48;
    EXPECT_EQ("+12:00", *quaterHoursToZoneOffset(&value));
  }

  TEST(quaterHoursToZoneOffset, invalid)
  {
    EXPECT_FALSE(quaterHoursToZoneOffset(nullptr));
  }
}
