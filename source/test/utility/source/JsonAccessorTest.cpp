#include <gtest/gtest.h>

#include "lib/utility/include/JsonAccessor.h"

namespace utility
{

  TEST(JsonAccessor, missingField)
  {
    EXPECT_FALSE(getString(json::parse(R"({"a":"v"})"), "m"));
  }

  TEST(JsonAccessor, topLevelString)
  {
    EXPECT_EQ("v", getString(json::parse(R"({"a":"v"})"), "a"));
  }

  TEST(JsonAccessor, nestedString)
  {
    EXPECT_EQ("v", getString(json::parse(R"({"a":{"b":"v"}})"), "a", "b"));
  }
}