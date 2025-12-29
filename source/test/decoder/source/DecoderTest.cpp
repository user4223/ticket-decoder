// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "lib/decoder/api/include/Decoder.h"

#include "lib/dip/include/Transform.h"

#include "test/support/include/TestSupport.h"

#include <gtest/gtest.h>

#include <opencv2/core/mat.hpp>

#include <string>

namespace decoder::api
{
  TEST(UIC918_3_City_Mobil, decode)
  {
    auto &testSupport = ::test::support::get();
    auto image = testSupport.getDecoderImage("Muster 918-3 City-Mobil Ticket_0_decoded.jpg");
    auto const result = Decoder::create(testSupport.getContext(), {false, true})->decode(23, cv::Rect{}, image);
#ifdef WITH_BARCODE_DECODER
    ASSERT_EQ(result.level, Level::Decoded);
    ASSERT_EQ(result.payload.size(), 353);
    EXPECT_EQ(result.payload[0], '#');
    EXPECT_EQ(result.payload[1], 'U');
    EXPECT_EQ(result.payload[2], 'T');
#else
    ASSERT_EQ(result.level, Level::Unknown);
#endif
  }

  TEST(UIC918_9_Laenderticket_Schleswig_Holstein, decode)
  {
    auto &testSupport = ::test::support::get();
    auto image = testSupport.getDecoderImage("Muster 918-9 Länderticket Schleswig-Holstein_0_decoded.jpg");
    image = dip::rotate(image, 4);
    auto const result = Decoder::create(testSupport.getContext(), {false, true})->decode(23, cv::Rect{}, image);
#ifdef WITH_BARCODE_DECODER
    ASSERT_EQ(result.level, Level::Decoded);
    ASSERT_EQ(result.payload.size(), 425);
    EXPECT_EQ(result.payload[0], '#');
    EXPECT_EQ(result.payload[1], 'U');
    EXPECT_EQ(result.payload[2], 'T');
#else
    ASSERT_EQ(result.level, Level::Unknown);
#endif
  }
}
