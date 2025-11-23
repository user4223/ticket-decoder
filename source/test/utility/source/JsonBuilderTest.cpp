// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/utility/include/JsonBuilder.h"

namespace utility
{

  TEST(JsonBuilder, addNative)
  {
    EXPECT_EQ(R"({"a":4})", JsonBuilder::object().add("a", 4).buildString());
    EXPECT_EQ(R"({"a":true})", JsonBuilder::object().add("a", true).buildString());
    EXPECT_EQ(R"({"a":"v"})", JsonBuilder::object().add("a", std::string("v")).buildString());
  }

  TEST(JsonBuilder, addObject)
  {
    EXPECT_EQ(R"({"a":{"b":"v"}})", JsonBuilder::object()
                                        .add("a", JsonBuilder::object()
                                                      .add("b", std::string("v")))
                                        .buildString());
  }

  TEST(JsonBuilder, addArray)
  {
    EXPECT_EQ(R"({"a":["b","v"]})", JsonBuilder::object()
                                        .add("a", JsonBuilder::array()
                                                      .add(std::string("b"))
                                                      .add(std::string("v")))
                                        .buildString());
  }
}