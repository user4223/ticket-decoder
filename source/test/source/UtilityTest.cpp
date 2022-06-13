
#include <gtest/gtest.h>

#include "lib/uic918/include/Utility.h"

TEST(getNumeric, readOddBytes)
{
  auto const source = std::vector<std::uint8_t>{0xff, 0, 0, 1, 0xff}; // big endian 1
  auto position = source.begin() + 1;
  EXPECT_EQ(Utility::getNumeric24(position), 1);
}

TEST(getDateTimeCompact, initial)
{
  auto const source = std::vector<std::uint8_t>{0x28, 0x39, 0x70, 0x62};
  auto position = source.begin();
  EXPECT_EQ(Utility::getDateTimeCompact(position), "2010-01-25T14:03:02");
}