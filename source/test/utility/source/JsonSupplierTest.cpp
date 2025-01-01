#include <gtest/gtest.h>

#include "lib/utility/include/JsonSupplier.h"

namespace utility
{

  TEST(JsonSupplier, missingField)
  {
    EXPECT_FALSE(getString(R"({})"_json, "a"));
    EXPECT_FALSE(getString(R"({"a":"v"})"_json, "m"));
    EXPECT_FALSE(getString(R"({"a":{"b":"v"}})"_json, "a", "c"));
  }

  TEST(JsonSupplier, wrongType)
  {
    EXPECT_FALSE(getString(R"({"a":23})"_json, "a"));
    EXPECT_FALSE(getString(R"({"a":true})"_json, "a"));
    EXPECT_FALSE(getString(R"({"a":{"b": "v"}})"_json, "a"));
    EXPECT_FALSE(getString(R"(["a",23])"_json, 1));
    EXPECT_FALSE(getString(R"([true])"_json, 0));
  }

  TEST(JsonSupplier, getString)
  {
    EXPECT_EQ("v", getString(R"({"a":"v"})"_json, "a"));
  }

  TEST(JsonSupplier, getNestedString)
  {
    auto const json = R"({"a":{"b":"v"}})"_json;
    EXPECT_EQ("v", getString(json, "a", "b"));
    EXPECT_EQ(std::nullopt, getString(json, "a", "c"));
    EXPECT_EQ(std::nullopt, getString(json, "z", "c"));
  }

  TEST(JsonSupplier, getBool)
  {
    EXPECT_EQ(true, getBool(R"({"a":true})"_json, "a").value());
    EXPECT_EQ(false, getBool(R"({"a":false})"_json, "a").value());
    EXPECT_EQ(std::nullopt, getBool(R"({"a":"v"})"_json, "a"));
  }

  TEST(JsonSupplier, getNestedBool)
  {
    EXPECT_EQ(false, getBool(R"({"a":{"b":false}})"_json, "a", "b").value());
    EXPECT_EQ(true, getBool(R"({"a":{"b":[true, false]}})"_json, "a", "b", 0).value());
    EXPECT_EQ(false, getBool(R"({"a":{"b":[true, false]}})"_json, "a", "b", 1).value());
    EXPECT_EQ(std::nullopt, getBool(R"({"a":{"b":false}})"_json, "a", "c"));
    EXPECT_EQ(std::nullopt, getBool(R"({"a":{"b":false}})"_json, "z", "c"));
  }

  TEST(JsonSupplier, inArrayString)
  {
    auto const json = R"({"a":["v"]})"_json;
    EXPECT_EQ("v", getString(json, "a", 0));
    EXPECT_EQ(std::nullopt, getString(json, "a", 1));
    EXPECT_EQ(std::nullopt, getString(json, "a", -23));

    EXPECT_EQ("a", getString(R"(["a",23])"_json, 0));
  }

  TEST(JsonSupplier, ifStringExists)
  {
    auto value = std::optional<std::string>{};
    EXPECT_TRUE(ifString(R"({"a":{"b":"v"}})"_json, "a", "b")([&](auto const &s)
                                                              { value = s; }));
    EXPECT_TRUE(value);
    EXPECT_EQ("v", *value);
  }

  TEST(JsonSupplier, ifStringNotExists)
  {
    auto value = std::optional<std::string>{};
    EXPECT_FALSE(ifString(R"({"a":{"b":"v"}})"_json, "a", "n")([&](auto const &s)
                                                               { value = s; }));
    EXPECT_FALSE(value);
  }

  TEST(JsonSupplier, forEachBooleanArrayItem)
  {
    auto values = std::vector<bool>{};
    EXPECT_TRUE(forEachArrayItem(R"([true, false, true])"_json)([&](auto const &b)
                                                                { values.push_back(b); }));
    EXPECT_EQ(std::vector<bool>({true, false, true}), values);
  }

  TEST(JsonSupplier, forEachStringArrayItem)
  {
    auto values = std::vector<std::string>{};
    EXPECT_TRUE(forEachArrayItem(R"(["a", "b", "c"])"_json)([&](auto const &b)
                                                            { values.push_back(b); }));
    EXPECT_EQ(std::vector<std::string>({"a", "b", "c"}), values);
  }

  TEST(JsonSupplier, forEachEmptyArrayItem)
  {
    auto values = std::vector<bool>{};
    EXPECT_TRUE(forEachArrayItem(R"([])"_json)([&](auto const &b)
                                               { values.push_back(b); }));
    EXPECT_EQ(0, values.size());
  }

  TEST(JsonSupplier, forEachNonArrayItem)
  {
    auto values = std::vector<bool>{};
    EXPECT_FALSE(forEachArrayItem(R"({"a": true})"_json)([&](auto const &b)
                                                         { values.push_back(b); }));
    EXPECT_EQ(0, values.size());
  }

  TEST(JsonSupplier, forEachNestedArrayItem)
  {
    auto values = std::vector<bool>{};
    EXPECT_TRUE(forEachArrayItem(R"({"a":[true, false, true]})"_json, "a")([&](auto const &b)
                                                                           { values.push_back(b); }));
    EXPECT_EQ(3, values.size());
  }
}
