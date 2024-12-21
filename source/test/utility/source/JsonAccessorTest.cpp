#include <gtest/gtest.h>

#include "lib/utility/include/JsonAccessor.h"

namespace utility
{

  TEST(JsonAccessor, missingField)
  {
    EXPECT_FALSE(getString(R"({})"_json, "a"));
    EXPECT_FALSE(getString(R"({"a":"v"})"_json, "m"));
    EXPECT_FALSE(getString(R"({"a":{"b":"v"}})"_json, "a", "c"));
  }

  TEST(JsonAccessor, wrongType)
  {
    EXPECT_FALSE(getString(R"({"a":23})"_json, "a"));
    EXPECT_FALSE(getString(R"({"a":true})"_json, "a"));
    EXPECT_FALSE(getString(R"({"a":{"b": "v"}})"_json, "a"));
    EXPECT_FALSE(getString(R"(["a",23])"_json, 1));
    EXPECT_FALSE(getString(R"([true])"_json, 0));
  }

  TEST(JsonAccessor, topLevelString)
  {
    EXPECT_EQ("v", getString(R"({"a":"v"})"_json, "a"));
  }

  TEST(JsonAccessor, nestedString)
  {
    auto const json = R"({"a":{"b":"v"}})"_json;
    EXPECT_EQ("v", getString(json, "a", "b"));
    EXPECT_EQ(std::nullopt, getString(json, "a", "c"));
  }

  TEST(JsonAccessor, inArrayString)
  {
    auto const json = R"({"a":["v"]})"_json;
    EXPECT_EQ("v", getString(json, "a", 0));
    EXPECT_EQ(std::nullopt, getString(json, "a", 1));
    EXPECT_EQ(std::nullopt, getString(json, "a", -23));

    EXPECT_EQ("a", getString(R"(["a",23])"_json, 0));
  }

  TEST(JsonAccessor, ifStringExists)
  {
    auto value = std::optional<std::string>{};
    ifString(R"({"a":{"b":"v"}})"_json, "a", "b")([&](auto const &s)
                                                  { value = s; });
    EXPECT_TRUE(value);
    EXPECT_EQ("v", *value);
  }

  TEST(JsonAccessor, ifStringNotExists)
  {
    auto value = std::optional<std::string>{};
    ifString(R"({"a":{"b":"v"}})"_json, "a", "n")([&](auto const &s)
                                                  { value = s; });
    EXPECT_FALSE(value);
  }
}
