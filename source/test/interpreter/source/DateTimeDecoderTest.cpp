// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/DateTimeDecoder.h"

namespace interpreter::detail::common
{
  TEST(DateTimeDecoder, consumeDateTimeCompact4)
  {
    auto context = Context({0x28, 0x39, 0x70, 0x62});
    EXPECT_EQ(DateTimeDecoder::consumeDateTimeCompact4(context), "2010-01-25T14:03:02");
  }

  TEST(DateTimeDecoder, consumeDateTime12)
  {
    auto context = Context({'2', '7', '1', '0', '2', '0', '2', '0', '1', '3', '4', '5'});
    EXPECT_EQ(DateTimeDecoder::consumeDateTime12(context), "2020-10-27T13:45:00");
  }

  TEST(DateTimeDecoder, consumeDate8)
  {
    auto context = Context({'1', '3', '0', '1', '2', '0', '2', '1'});
    EXPECT_EQ(DateTimeDecoder::consumeDate8(context), "2021-01-13");
  }

  TEST(DateTimeDecoder, consumeDate8Null)
  {
    auto context = Context({0, 0, 0, 0, 0, 0, 0, 0});
    EXPECT_EQ(DateTimeDecoder::consumeDate8(context), "2021-01-13");
  }
}
