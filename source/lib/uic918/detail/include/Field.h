#pragma once

#include <string>
#include <optional>

namespace uic918::detail
{
  struct Field
  {
    std::string value;
    std::optional<std::string> description;

    std::string toString() const
    {
      return description ? value + " (" + description.value() + ")" : value;
    }

    operator std::string() const
    {
      return toString();
    }
  };
}
