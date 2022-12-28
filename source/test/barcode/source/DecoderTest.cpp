
#include "lib/barcode/api/include/Decoder.h"
#include "lib/utility/include/Logging.h"

#include <opencv2/core/mat.hpp>

#include <string>

#include "test/support/include/Loader.h"

#include <gtest/gtest.h>

namespace barcode::api
{
  TEST(UIC918_3_City_Mobil, decode)
  {
    auto image = ::support::Loader::getImage("Muster 918-3 City-Mobil Ticket_0_decoded.jpg");
    auto loggerFactory = ::utility::LoggerFactory::create();
    auto const result = Decoder::decode(loggerFactory, 23, cv::Rect{}, image, false);
    EXPECT_EQ(result.level, Level::Decoded);
    EXPECT_EQ(result.payload.size(), 353);
    EXPECT_EQ(result.payload[0], '#');
    EXPECT_EQ(result.payload[1], 'U');
    EXPECT_EQ(result.payload[2], 'T');
  }

  TEST(UIC918_9_Laenderticket_Schleswig_Holstein, decode)
  {
    auto image = ::support::Loader::getImage("Muster 918-9 Länderticket Schleswig-Holstein_0_decoded.jpg");
    auto loggerFactory = ::utility::LoggerFactory::create();
    auto const result = Decoder::decode(loggerFactory, 23, cv::Rect{}, image, false);
    EXPECT_EQ(result.level, Level::Decoded);
    EXPECT_EQ(result.payload.size(), 425);
    EXPECT_EQ(result.payload[0], '#');
    EXPECT_EQ(result.payload[1], 'U');
    EXPECT_EQ(result.payload[2], 'T');
  }
}
