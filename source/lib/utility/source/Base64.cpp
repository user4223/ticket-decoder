// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Base64.h"

#include <boost/beast/core/detail/base64.hpp>

namespace utility::base64
{
  namespace bb = boost::beast::detail::base64;

  std::vector<std::uint8_t> decode(std::string const &encoded)
  {
    if (encoded.empty())
    {
      return {};
    }

    auto destination = std::vector<std::uint8_t>(bb::decoded_size(encoded.length()));
    auto result = bb::decode((void *)destination.data(), encoded.data(), encoded.length());
    destination.resize(result.first);
    return destination;
  }

  std::string encode(std::vector<std::uint8_t> const &in)
  {
    return encode(in.data(), in.size());
  }

  std::string encode(std::uint8_t const *const data, size_t size)
  {
    if (size == 0)
    {
      return {};
    }

    auto destination = std::string();
    destination.resize(bb::encoded_size(size));
    auto result = bb::encode(destination.data(), data, size);
    destination.resize(result);
    return destination;
  }
}
