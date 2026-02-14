// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/Context.h"
#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

namespace interpreter::detail::vdv
{
  /* Multi-byte integers are big-endian encoded, see DER ASN.1
   */
  std::uint32_t consumeLength(common::Context &context)
  {
    auto const first = common::consumeInteger1(context);
    // clang-format off
    if (first < 0x80) { return first; }
    else if (first == 0x81) { return common::consumeInteger1(context); }
    else if (first == 0x82) { return common::consumeInteger2(context); }
    else if (first == 0x83) { return common::consumeInteger3(context); }
    else if (first == 0x84) { return common::consumeInteger4(context); }
    // clang-format on
    throw std::runtime_error(std::string("Found unexpected length indicator tag (expecting x<0x80 or x=0x8y with y=<remaining bytes>): ") + std::to_string(first));
  }

  common::Context &consumeExpectedTag(common::Context &context, common::TLVTag const &expectedTag)
  {
    auto tag = common::TLVDecoder::consumeTag(context);
    ensureTag(tag, expectedTag);
    return context;
  }

  common::Context &consumeExpectedEndTag(common::Context &context, common::TLVTag const &expectedTag)
  {
    // TODO This is working only when expectedTag is just a single byte, refactor this to make it generic
    auto const tag = common::TLVTag{context.consumeBytesEnd(1)[0]};
    ensureTag(tag, expectedTag);
    return context;
  }

  common::Context &consumeExpectedFrameTags(common::Context &context, common::TLVTag const &expectedBeginTag, common::TLVTag const &expectedEndTag)
  {
    consumeExpectedTag(context, expectedBeginTag);
    consumeExpectedEndTag(context, expectedEndTag);
    return context;
  }

  std::span<std::uint8_t const> consumeExpectedTagValue(common::Context &context, common::TLVTag const &expectedTag)
  {
    return context.consumeBytes(consumeLength(consumeExpectedTag(context, expectedTag)));
  }

  std::span<std::uint8_t const> consumeExpected(common::Context &context, std::vector<std::uint8_t> expectedValue)
  {
    auto const value = context.consumeBytes(expectedValue.size());
    if (!std::equal(value.begin(), value.end(), expectedValue.begin(), expectedValue.end()))
    {
      throw std::runtime_error(std::string("Unexpected value found: ") + common::bytesToHexString(value));
    }
    return value;
  }

  bool peekExpected(common::Context &context, std::vector<std::uint8_t> expectedValue)
  {

    auto const value = context.peekBytes(expectedValue.size());
    return std::equal(value.begin(), value.end(), expectedValue.begin(), expectedValue.end());
  }

  void ensureTag(common::TLVTag const &tag, common::TLVTag const &expectedTag)
  {
    if (tag != expectedTag)
    {
      throw std::runtime_error(std::string("Unexpected tag found: ") + common::bytesToHexString(tag));
    }
  }
}
