// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <optional>

namespace interpreter::detail::common
{
  class Field
  {
    std::string value;
    std::optional<std::string> description;

  public:
    Field(std::string v)
        : value(std::move(v)), description(std::nullopt)
    {
    }

    Field(std::string v, std::optional<std::string> d)
        : value(std::move(v)), description(std::move(d))
    {
    }

    std::string getValue() const
    {
      return value;
    }

    operator std::string() const
    {
      return getValue();
    }

    std::optional<std::string> const &getDescription() const
    {
      return description;
    }
  };
}
