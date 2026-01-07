// SPDX-FileCopyrightText: (C) 2025 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/VDVUtility.h"

#include "lib/interpreter/detail/common/include/InterpreterUtility.h"

namespace interpreter::detail::vdv
{
  /* Multi-byte integers are big-endian encoded, see DER ASN.1
   */
  std::uint32_t getLength(common::Context &context)
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

  TagType getTag(common::Context &context)
  {
    auto const first = common::getNumeric8(context);
    if (first == 0x7f || first == 0x5f)
    {
      return {first, common::getNumeric8(context)};
    }

    return {first, 0};
  }

  common::Context &ensureExpectedTag(common::Context &context, TagType expectedTag)
  {
    auto tag = getTag(context);
    if (tag != expectedTag)
    {
      throw std::runtime_error(std::string("Unexpected tag found: ") + common::bytesToHexString(tag));
    }
    return context;
  }

  void ensureEmpty(common::Context const &context)
  {
    if (!context.isEmpty())
    {
      throw std::runtime_error(std::string("Expecting fully consumed context, but found remaining bytes: ") + std::to_string(context.getRemainingSize()));
    }
  }
}
