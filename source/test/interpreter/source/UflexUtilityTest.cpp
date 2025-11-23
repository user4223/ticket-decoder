// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/uic918/u_flex/include/UflexUtility.h"

namespace interpreter::detail::uic::u_flex
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

    EXPECT_EQ(daysAndYearToIsoDate(2023l, 365l), "2023-12-31");
    EXPECT_EQ(daysAndYearToIsoDate(2023l, 366l), "2024-01-01");

    EXPECT_EQ(daysAndYearToIsoDate(2023l, 365l + 366l), "2024-12-31");
    EXPECT_EQ(daysAndYearToIsoDate(2023l, 365l + 59l), "2024-02-28");
    EXPECT_EQ(daysAndYearToIsoDate(2023l, 365l + 60l), "2024-02-29");
    EXPECT_EQ(daysAndYearToIsoDate(2023l, 365l + 61l), "2024-03-01");

    EXPECT_EQ(daysAndYearToIsoDate(2024l, 366l + 365l), "2025-12-31");
    EXPECT_EQ(daysAndYearToIsoDate(2024l, 366l + 59l), "2025-02-28");
    EXPECT_EQ(daysAndYearToIsoDate(2024l, 366l + 60l), "2025-03-01");

    EXPECT_EQ(daysAndYearToIsoDate(2025l, 365l + 365l), "2026-12-31");
    EXPECT_EQ(daysAndYearToIsoDate(2025l, 365l + 59l), "2026-02-28");
    EXPECT_EQ(daysAndYearToIsoDate(2025l, 365l + 60l), "2026-03-01");
  }

  TEST(daysAndYearToIsoDate, multipleYearOverflowThrows)
  {
    EXPECT_THROW(daysAndYearToIsoDate(2023l, 365l + 367l), std::runtime_error);
    EXPECT_NO_THROW(daysAndYearToIsoDate(2023l, 365l + 366l));

    EXPECT_THROW(daysAndYearToIsoDate(2024l, 366l + 366l), std::runtime_error);
    EXPECT_NO_THROW(daysAndYearToIsoDate(2024l, 366l + 365l));

    EXPECT_THROW(daysAndYearToIsoDate(2025l, 365l + 366l), std::runtime_error);
    EXPECT_NO_THROW(daysAndYearToIsoDate(2025l, 365l + 365l));
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

  TEST(quaterHoursToIsoZone, valid)
  {
    auto value = -4l;
    EXPECT_EQ("-01:00", *quaterHoursToIsoZone(&value));
    value = 4;
    EXPECT_EQ("+01:00", *quaterHoursToIsoZone(&value));
    value = 3;
    EXPECT_EQ("+00:45", *quaterHoursToIsoZone(&value));
    value = 1;
    EXPECT_EQ("+00:15", *quaterHoursToIsoZone(&value));
    value = 0;
    EXPECT_EQ("+00:00", *quaterHoursToIsoZone(&value));
    value = -8;
    EXPECT_EQ("-02:00", *quaterHoursToIsoZone(&value));
    value = -48;
    EXPECT_EQ("-12:00", *quaterHoursToIsoZone(&value));
    value = 48;
    EXPECT_EQ("+12:00", *quaterHoursToIsoZone(&value));
  }

  TEST(quaterHoursToIsoZone, invalid)
  {
    EXPECT_FALSE(quaterHoursToIsoZone(nullptr));
  }
}
