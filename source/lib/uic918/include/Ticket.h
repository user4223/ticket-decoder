#pragma once

#include <optional>
#include <string>

struct Ticket
{
  virtual std::optional<std::string> getGivenName() = 0;

  virtual std::optional<std::string> getFamilyName() = 0;

  virtual std::optional<std::string> getUniqueId() = 0;
};
