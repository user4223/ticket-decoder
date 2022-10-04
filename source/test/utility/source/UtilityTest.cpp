
#include <gtest/gtest.h>

#include "lib/utility/include/Utility.h"

namespace utility
{

  TEST(safeIncrement, untilMax)
  {
    unsigned int value = 0;
    EXPECT_EQ(1, safeIncrement(value, 3));
    EXPECT_EQ(2, safeIncrement(value, 3));
    EXPECT_EQ(3, safeIncrement(value, 3));
    EXPECT_EQ(3, safeIncrement(value, 3)); // stays 3
  }

  TEST(safeIncrement, untilMax2)
  {
    unsigned int value = 0;
    EXPECT_EQ(2, safeIncrement(value, 2, 3));
    EXPECT_EQ(3, safeIncrement(value, 2, 3));
    EXPECT_EQ(3, safeIncrement(value, 2, 3)); // stays 3
  }

  TEST(safeIncrement, beyondMax)
  {
    unsigned int value = 4;
    EXPECT_EQ(3, safeIncrement(value, 3));      // stays 3
    EXPECT_EQ(3, safeIncrement(value, 2, 3));   // stays 3
    EXPECT_EQ(3, safeIncrement(value, 100, 3)); // stays 3
  }

  TEST(safeDecrement, untilMin)
  {
    unsigned int value = 3;
    EXPECT_EQ(2, safeDecrement(value, 2));
    EXPECT_EQ(2, safeDecrement(value, 2));

    EXPECT_EQ(1, safeDecrement(value, 0));
    EXPECT_EQ(0, safeDecrement(value, 0));
    EXPECT_EQ(0, safeDecrement(value, 0)); // stays 0
  }

}
