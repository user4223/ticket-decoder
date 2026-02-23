// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

namespace interpreter::detail::common
{
  TEST(getNumeric, min8)
  {
    auto context = Context({0xff, 1, 0xff});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger1(context), 1);
  }

  TEST(getNumeric, max8)
  {
    auto context = Context({0xfe, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger1(context), 255);
  }

  TEST(getNumeric, min16)
  {
    auto context = Context({0xff, 0, 1, 0xff});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger2(context), 1);
  }

  TEST(getNumeric, max16)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger2(context), 65535);
  }

  TEST(getNumeric, min24)
  {
    auto context = Context({0xff, 0, 0, 1, 0xff}); // big endian 1
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger3(context), 1);
  }

  TEST(getNumeric, max24)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger3(context), 16777215);
  }

  TEST(getNumeric, min32)
  {
    auto context = Context({0xff, 0, 0, 0, 1, 0xff}); // big endian 1
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger4(context), 1);
  }

  TEST(getNumeric, max32)
  {
    auto context = Context({0xfe, 0xff, 0xff, 0xff, 0xff, 0xfe});
    context.ignoreBytes(1);
    EXPECT_EQ(NumberDecoder::consumeInteger4(context), 4294967295);
  }
}
