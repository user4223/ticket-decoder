// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/StringDecoder.h"

namespace interpreter::detail::common
{
  TEST(StringDecoder, consumeUtf8)
  {
    auto const buffer = std::string("öäü");
    auto context = Context(std::span<std::uint8_t const>{(std::uint8_t const *)buffer.data(), 6});
    EXPECT_EQ(StringDecoder::consumeUTF8(context, 6), std::string("öäü"));

    context = Context({0xC3, 0xBC, 0xC3, 0xB6, 0xC3, 0xA4, 0xC3, 0x9F, 0xC3, 0x9C, 0xC3, 0x96, 0xC3, 0x84});
    EXPECT_EQ(StringDecoder::consumeUTF8(context, 6), std::string("üöä"));
    EXPECT_EQ(StringDecoder::consumeUTF8(context, 8), std::string("ßÜÖÄ"));
  }

  TEST(StringDecoder, consumeUtf8StopAtNull)
  {
    auto context = Context({'R', 'P', 'E', 'X', '4', 'F', '-', '4', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    EXPECT_EQ(StringDecoder::consumeUTF8(context, 20), std::string("RPEX4F-4"));
  }

  TEST(StringDecoder, consumeUtf8TrimTrailingSpaces)
  {
    auto context = Context({'A', 'B', 'C', ' ', '\n', ' ', ' ', ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    EXPECT_EQ(StringDecoder::consumeUTF8(context, 20), std::string("ABC"));
  }

  TEST(StringDecoder, consumeUtf8All)
  {
    auto context = Context({'R', 'P', 'E', 'X', '4', 'F', '-', '4'});
    EXPECT_EQ(StringDecoder::consumeUTF8(context, 8), std::string("RPEX4F-4"));
  }

  TEST(StringDecoder, consumeUtf8Empty)
  {
    auto context = Context({0});
    EXPECT_EQ(StringDecoder::consumeUTF8(context, 8), std::string(""));
  }

  TEST(StringDecoder, consumeLatin1)
  {
    auto context = Context({0xf6, 0xe4, 0xfc});
    EXPECT_EQ(StringDecoder::consumeLatin1(context, 6), std::string("öäü"));

    context = Context({0xfc, 0xf6, 0xe4, 0xdf, 0xdc, 0xd6, 0xc4});
    EXPECT_EQ(StringDecoder::consumeLatin1(context, 3), std::string("üöä"));
    EXPECT_EQ(StringDecoder::consumeLatin1(context, 4), std::string("ßÜÖÄ"));
  }

  TEST(StringDecoder, consumeLatin1StopAtNull)
  {
    auto context = Context({'R', 'P', 'E', 'X', '4', 'F', '-', '4', 0});
    EXPECT_EQ(StringDecoder::consumeLatin1(context, 20), std::string("RPEX4F-4"));
  }

  TEST(StringDecoder, consumeLatin1TrimTrailingSpaces)
  {
    auto context = Context({'A', 'B', 'C', ' ', '\n', ' ', ' ', ' ', 0});
    EXPECT_EQ(StringDecoder::consumeLatin1(context, 20), std::string("ABC"));
  }

  TEST(StringDecoder, consumeLatin1All)
  {
    auto context = Context({'R', 'P', 'E', 'X', '4', 'F', '-', '4'});
    EXPECT_EQ(StringDecoder::consumeLatin1(context, 8), std::string("RPEX4F-4"));
  }

  TEST(StringDecoder, consumeLatin1Empty)
  {
    auto context = Context({0});
    EXPECT_EQ(StringDecoder::consumeLatin1(context, 8), std::string(""));
  }

  TEST(StringDecoder, isASCIIPrintable)
  {
    auto const data = std::vector<std::uint8_t>{0x00, 0x1f, ' ', '~', 0x7f, 0x80, 0xff};
    EXPECT_FALSE(StringDecoder::isASCII({&(data[0]), 1}, true));
    EXPECT_FALSE(StringDecoder::isASCII({&(data[1]), 1}, true));
    EXPECT_TRUE(StringDecoder::isASCII({&(data[2]), 1}, true));
    EXPECT_TRUE(StringDecoder::isASCII({&(data[3]), 1}, true));
    EXPECT_FALSE(StringDecoder::isASCII({&(data[4]), 1}, true));
    EXPECT_FALSE(StringDecoder::isASCII({&(data[5]), 1}, true));
    EXPECT_FALSE(StringDecoder::isASCII({&(data[6]), 1}, true));
  }

  TEST(StringDecoder, isASCII)
  {
    auto const data = std::vector<std::uint8_t>{0x00, 0x1f, ' ', '~', 0x7f, 0x80, 0xff};
    EXPECT_TRUE(StringDecoder::isASCII({&(data[0]), 1}, false));
    EXPECT_TRUE(StringDecoder::isASCII({&(data[1]), 1}, false));
    EXPECT_TRUE(StringDecoder::isASCII({&(data[2]), 1}, false));
    EXPECT_TRUE(StringDecoder::isASCII({&(data[3]), 1}, false));
    EXPECT_TRUE(StringDecoder::isASCII({&(data[4]), 1}, false));
    EXPECT_FALSE(StringDecoder::isASCII({&(data[5]), 1}, false));
    EXPECT_FALSE(StringDecoder::isASCII({&(data[6]), 1}, false));
  }

  TEST(StringDecoder, consumeASCII)
  {
    auto context = Context({'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'});
    EXPECT_EQ(StringDecoder::consumeASCII(context, 100), std::string("hello world!"));
  }

  TEST(StringDecoder, consumeASCIIInvalid)
  {
    auto context = Context({'h', 'e', 'l', 'l', 0xf6, ' ', 'w', 'o', 'r', 'l', 'd', '!'});
    EXPECT_THROW(StringDecoder::consumeASCII(context, 100), std::runtime_error);
  }

  TEST(StringDecoder, consumeASCIIPrintable)
  {
    auto context = Context({0x20, '!', 'a', 'A', '0', '9', 'z', 'Z', 0x7E});
    EXPECT_EQ(StringDecoder::consumeASCII(context, 9, true), std::string(" !aA09zZ~"));
  }

  TEST(StringDecoder, consumeASCIIEnsurePrintableTrue)
  {
    auto context = Context({0x00, 0x1F, 0x20, 0x7E, 0x7F, 0xFF});
    EXPECT_THROW(StringDecoder::consumeASCII(context, 1, true), std::runtime_error);
    EXPECT_THROW(StringDecoder::consumeASCII(context, 1, true), std::runtime_error);
    EXPECT_NO_THROW(StringDecoder::consumeASCII(context, 1, true));
    EXPECT_NO_THROW(StringDecoder::consumeASCII(context, 1, true));
    EXPECT_THROW(StringDecoder::consumeASCII(context, 1, true), std::runtime_error);
    EXPECT_THROW(StringDecoder::consumeASCII(context, 1, true), std::runtime_error);
    EXPECT_TRUE(context.isEmpty());
  }

  TEST(StringDecoder, consumeASCIIEnsurePrintableFalse)
  {
    auto context = Context({0x00, 0x1F, 0x20, 0x7E, 0x7F, 0x80, 0xFF});
    EXPECT_NO_THROW(StringDecoder::consumeASCII(context, 1, false));
    EXPECT_NO_THROW(StringDecoder::consumeASCII(context, 1, false));
    EXPECT_NO_THROW(StringDecoder::consumeASCII(context, 1, false));
    EXPECT_NO_THROW(StringDecoder::consumeASCII(context, 1, false));
    EXPECT_NO_THROW(StringDecoder::consumeASCII(context, 1, false));
    EXPECT_THROW(StringDecoder::consumeASCII(context, 1, false), std::runtime_error);
    EXPECT_THROW(StringDecoder::consumeASCII(context, 1, false), std::runtime_error);
    EXPECT_TRUE(context.isEmpty());
  }

  TEST(StringDecoder, filledVectorToHexString)
  {
    EXPECT_EQ(StringDecoder::toHexString(std::vector<std::uint8_t>{0x23}), "23");
    EXPECT_EQ(StringDecoder::toHexString(std::vector<std::uint8_t>(4, ' ')), "20202020");
    EXPECT_EQ(StringDecoder::toHexString(std::vector<std::uint8_t>(3)), "000000");
    EXPECT_EQ(StringDecoder::toHexString(std::vector<std::uint8_t>{0x12, 0x0A, 0xAB, 0x00, 0xFF}), "120AAB00FF");
  }

  TEST(StringDecoder, emptyVectorToHexString)
  {
    EXPECT_EQ(StringDecoder::toHexString(std::vector<std::uint8_t>{}), "");
  }

  TEST(StringDecoder, filledArrayToHexString)
  {
    EXPECT_EQ(StringDecoder::toHexString(std::array<std::uint8_t, 1>{}), "00");
    EXPECT_EQ(StringDecoder::toHexString(std::array<std::uint8_t, 3>{0x12, 0x0A, 0xAB}), "120AAB");
    EXPECT_EQ(StringDecoder::toHexString(std::array<std::uint8_t, 3>{0x12, 0x0A}), "120A00");
    EXPECT_EQ(StringDecoder::toHexString(std::array<std::uint8_t, 3>{}), "000000");
    EXPECT_EQ(StringDecoder::toHexString(std::array<std::uint8_t, 5>{0x11, 0x22, 0x33, 0x44, 0x55}), "1122334455");
  }

  TEST(StringDecoder, emptyArrayToHexString)
  {
    EXPECT_EQ(StringDecoder::toHexString(std::array<std::uint8_t, 0>{}), "");
  }

  TEST(StringDecoder, integer4ToHexString)
  {
    EXPECT_EQ(StringDecoder::toHexString(std::uint32_t()), "00000000");
    EXPECT_EQ(StringDecoder::toHexString(std::uint32_t(1)), "00000001");
    EXPECT_EQ(StringDecoder::toHexString(std::uint32_t(0xffffffff)), "FFFFFFFF");
    EXPECT_EQ(StringDecoder::toHexString(2323u), "00000913");
  }

  TEST(StringDecoder, integer2ToHexString)
  {
    EXPECT_EQ(StringDecoder::toHexString(std::uint16_t()), "0000");
    EXPECT_EQ(StringDecoder::toHexString(std::uint16_t(1)), "0001");
    EXPECT_EQ(StringDecoder::toHexString(std::uint16_t(0xffff)), "FFFF");
  }

  TEST(StringDecoder, integer1ToHexString)
  {
    EXPECT_EQ(StringDecoder::toHexString(std::uint8_t()), "00");
    EXPECT_EQ(StringDecoder::toHexString(std::uint8_t(1)), "01");
    EXPECT_EQ(StringDecoder::toHexString(std::uint8_t(0xff)), "FF");
  }
}
