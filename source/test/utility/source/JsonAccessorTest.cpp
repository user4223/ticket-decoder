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

  TEST(JsonAccessor, getString)
  {
    EXPECT_EQ("v", getString(R"({"a":"v"})"_json, "a"));
  }

  TEST(JsonAccessor, getNestedString)
  {
    auto const json = R"({"a":{"b":"v"}})"_json;
    EXPECT_EQ("v", getString(json, "a", "b"));
    EXPECT_EQ(std::nullopt, getString(json, "a", "c"));
    EXPECT_EQ(std::nullopt, getString(json, "z", "c"));
  }

  TEST(JsonAccessor, getBool)
  {
    EXPECT_EQ(true, getBool(R"({"a":true})"_json, "a").value());
    EXPECT_EQ(false, getBool(R"({"a":false})"_json, "a").value());
    EXPECT_EQ(std::nullopt, getBool(R"({"a":"v"})"_json, "a"));
  }

  TEST(JsonAccessor, getNestedBool)
  {
    EXPECT_EQ(false, getBool(R"({"a":{"b":false}})"_json, "a", "b").value());
    EXPECT_EQ(true, getBool(R"({"a":{"b":[true, false]}})"_json, "a", "b", 0).value());
    EXPECT_EQ(false, getBool(R"({"a":{"b":[true, false]}})"_json, "a", "b", 1).value());
    EXPECT_EQ(std::nullopt, getBool(R"({"a":{"b":false}})"_json, "a", "c"));
    EXPECT_EQ(std::nullopt, getBool(R"({"a":{"b":false}})"_json, "z", "c"));
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
    EXPECT_TRUE(ifString(R"({"a":{"b":"v"}})"_json, "a", "b")([&](auto const &s)
                                                              { value = s; }));
    EXPECT_TRUE(value);
    EXPECT_EQ("v", *value);
  }

  TEST(JsonAccessor, ifStringNotExists)
  {
    auto value = std::optional<std::string>{};
    EXPECT_FALSE(ifString(R"({"a":{"b":"v"}})"_json, "a", "n")([&](auto const &s)
                                                               { value = s; }));
    EXPECT_FALSE(value);
  }
}
