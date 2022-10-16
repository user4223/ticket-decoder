#include <gtest/gtest.h>

#include "lib/utility/include/JsonBuilder.h"

namespace utility
{

  TEST(JsonBuilder, addNative)
  {
    EXPECT_EQ(R"({"a":4})", JsonBuilder::object().add("a", 4).build());
    EXPECT_EQ(R"({"a":true})", JsonBuilder::object().add("a", true).build());
    EXPECT_EQ(R"({"a":"v"})", JsonBuilder::object().add("a", std::string("v")).build());
  }

  TEST(JsonBuilder, addObject)
  {
    EXPECT_EQ(R"({"a":{"b":"v"}})", JsonBuilder::object()
                                        .add("a", JsonBuilder::object()
                                                      .add("b", std::string("v")))
                                        .build());
  }

  TEST(JsonBuilder, addArray)
  {
    EXPECT_EQ(R"({"a":["b","v"]})", JsonBuilder::object()
                                        .add("a", JsonBuilder::array()
                                                      .add(std::string("b"))
                                                      .add(std::string("v")))
                                        .build());
  }
}