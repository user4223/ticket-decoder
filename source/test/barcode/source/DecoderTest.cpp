
#include "lib/barcode/api/include/Decoder.h"
#include "lib/utility/include/Logging.h"
#include "lib/dip/filtering/include/Transform.h"

#include <opencv2/core/mat.hpp>

#include <string>

#include "test/support/include/Loader.h"

#include <gtest/gtest.h>

namespace barcode::api
{
  static auto loggerFactory = utility::LoggerFactory::createLazy(true);

  TEST(UIC918_3_City_Mobil, decode)
  {
    auto image = ::support::Loader::getImage("Muster 918-3 City-Mobil Ticket_0_decoded.jpg");
    auto const result = Decoder::create(loggerFactory)->decode({false, true}, 23, cv::Rect{}, image);
    ASSERT_EQ(result.level, Level::Decoded);
    ASSERT_EQ(result.payload.size(), 353);
    EXPECT_EQ(result.payload[0], '#');
    EXPECT_EQ(result.payload[1], 'U');
    EXPECT_EQ(result.payload[2], 'T');
  }

  TEST(UIC918_9_Laenderticket_Schleswig_Holstein, decode)
  {
    auto image = ::support::Loader::getImage("Muster 918-9 Länderticket Schleswig-Holstein_0_decoded.jpg");
    image = dip::filtering::rotate(image, 4);
    auto const result = Decoder::create(loggerFactory)->decode({false, true}, 23, cv::Rect{}, image);
    ASSERT_EQ(result.level, Level::Decoded);
    ASSERT_EQ(result.payload.size(), 425);
    EXPECT_EQ(result.payload[0], '#');
    EXPECT_EQ(result.payload[1], 'U');
    EXPECT_EQ(result.payload[2], 'T');
  }
}
