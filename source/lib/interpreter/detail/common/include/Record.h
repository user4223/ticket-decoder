// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>

#include "lib/utility/include/JsonBuilder.h"

namespace interpreter::detail::common
{

  using json = nlohmann::json;

  class Record
  {
    std::string id;
    std::string version;
    json jsonValue;

  public:
    Record(std::string i, std::string v, ::utility::JsonBuilder &&jb)
        : id(i),
          version(v),
          jsonValue(jb.build())
    {
    }

    Record(Record const &) = delete;
    Record &operator=(Record const &) = delete;

    Record(Record &&) = default;
    Record &operator=(Record &&) = default;

    std::string const &getId() const
    {
      return id;
    }

    json getJson() const
    {
      return jsonValue;
    }
  };
}
