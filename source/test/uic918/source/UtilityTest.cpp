
#include <gtest/gtest.h>

#include "lib/uic918/detail/include/Utility.h"

namespace uic918::detail::utility
{
  TEST(getAlphanumeric, readAndStopAtNull) {
    auto const source = std::vector<std::uint8_t>{'R','P','E','X','4','F','-','4',0,0,0,0,0,0,0,0,0,0,0,0};
    auto position = source.begin();
    EXPECT_EQ(getAlphanumeric(position, 20), std::string("RPEX4F-4"));
  }

  TEST(getAlphanumeric, readAll) {
    auto const source = std::vector<std::uint8_t>{'R','P','E','X','4','F','-','4'};
    auto position = source.begin();
    EXPECT_EQ(getAlphanumeric(position, 8), std::string("RPEX4F-4"));
  }

  TEST(getNumeric, readOddBytes)
  {
    auto const source = std::vector<std::uint8_t>{0xff, 0, 0, 1, 0xff}; // big endian 1
    auto position = source.begin() + 1;
    EXPECT_EQ(getNumeric24(position), 1);
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

  TEST(toIsoDate, noLeapYear)
  {
    long const year = 2022;
    long day = 1;
    EXPECT_EQ(toIsoDate(&year, &day), "2022-01-01");
    day = 365;
    EXPECT_EQ(toIsoDate(&year, &day), "2022-12-31");
    day = 59;
    EXPECT_EQ(toIsoDate(&year, &day), "2022-02-28");
    day = 60;
    EXPECT_EQ(toIsoDate(&year, &day), "2022-03-01");
    day = 205;
    EXPECT_EQ(toIsoDate(&year, &day), "2022-07-24");
  }

  TEST(toIsoDate, leapYear)
  {
    long const year = 2020;
    long day = 1;
    EXPECT_EQ(toIsoDate(&year, &day), "2020-01-01");
    day = 366;
    EXPECT_EQ(toIsoDate(&year, &day), "2020-12-31");
    day = 60;
    EXPECT_EQ(toIsoDate(&year, &day), "2020-02-29");
    day = 61;
    EXPECT_EQ(toIsoDate(&year, &day), "2020-03-01");
    day = 316;
    EXPECT_EQ(toIsoDate(&year, &day), "2020-11-11");
  }
}
