
#include <gtest/gtest.h>

#include "lib/uic918/include/Utility.h"

TEST(getNumeric, readOddBytes)
{
  auto const source = std::vector<std::uint8_t>{0xff, 1, 0, 0, 0xff}; // little endian 1
  auto position = source.begin() + 1;
  EXPECT_EQ(Utility::getNumeric24(position), 1);
}