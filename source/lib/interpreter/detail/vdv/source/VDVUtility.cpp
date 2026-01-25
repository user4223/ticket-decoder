// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

namespace interpreter::detail::vdv
{
  /* Multi-byte integers are big-endian encoded, see DER ASN.1
   */
  std::uint32_t consumeLength(common::Context &context)
  {
    auto const first = common::getNumeric8(context);
    // clang-format off
    if (first < 0x80) { return first; }
    else if (first == 0x81) { return common::getNumeric8(context); }
    else if (first == 0x82) { return common::getNumeric16(context); }
    else if (first == 0x83) { return common::getNumeric24(context); }
    else if (first == 0x84) { return common::getNumeric32(context); }
    // clang-format on
    throw std::runtime_error(std::string("Found unexpected length indicator tag (expecting x<0x80 or x=0x8y with y=<remaining bytes>): ") + std::to_string(first));
  }

  TagType consumeTag(common::Context &context)
  {
    auto const first = common::getNumeric8(context);
    if (first == 0x7f || first == 0x5f)
    {
      return {first, common::getNumeric8(context)};
    }

    return {first, 0};
  }

  common::Context &consumeExpectedTag(common::Context &context, TagType const &expectedTag)
  {
    auto tag = consumeTag(context);
    ensureTag(tag, expectedTag);
    return context;
  }

  common::Context &consumeExpectedEndTag(common::Context &context, TagType const &expectedTag)
  {
    auto const tag = TagType{context.consumeBytesEnd(1)[0], 0};
    if (tag[0] == 0x7f || tag[0] == 0x5f)
    {
      throw std::runtime_error(std::string("Unexpected end tag found: ") + common::bytesToHexString(tag));
    }
    ensureTag(tag, expectedTag);
    return context;
  }

  common::Context &consumeExpectedFrameTags(common::Context &context, TagType const &expectedBeginTag, TagType const &expectedEndTag)
  {
    consumeExpectedTag(context, expectedBeginTag);
    consumeExpectedEndTag(context, expectedEndTag);
    return context;
  }

  std::span<std::uint8_t const> consumeExpectedTagValue(common::Context &context, TagType const &expectedTag)
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

  void ensureTag(TagType const &tag, TagType const &expectedTag)
  {
    if (tag != expectedTag)
    {
      throw std::runtime_error(std::string("Unexpected tag found: ") + common::bytesToHexString(tag));
    }
  }

  void ensureEmpty(common::Context const &context)
  {
    if (!context.isEmpty())
    {
      throw std::runtime_error(std::string("Expecting fully consumed context, but found remaining bytes: ") + std::to_string(context.getRemainingSize()));
    }
  }
}
