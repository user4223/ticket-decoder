// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>

namespace infrastructure
{
  class LoggerFactory;

  class Logger
  {
    std::string name;

    Logger(std::string name);

  public:
    friend LoggerFactory;

    Logger(Logger const &) = default;
    Logger(Logger &&) = default;
    Logger &operator=(Logger const &) = default;
    Logger &operator=(Logger &&) = default;

    char const *getName() const;
  };
}
