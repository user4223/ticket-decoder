// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Logger.h"

#include <string>

namespace infrastructure
{
  class LoggerFactory
  {
  private:
    LoggerFactory() = default;

  public:
    LoggerFactory(LoggerFactory &) = delete;
    LoggerFactory(LoggerFactory &&) = default;
    LoggerFactory &operator=(LoggerFactory &) = delete;
    LoggerFactory &operator=(LoggerFactory &&) = default;

    /* Pass __FILE__ here in normal cases to unify logger creation
     */
    Logger fromPath(std::string fullPath);

    static LoggerFactory create(bool verbose, bool toLogFile = false);

    static LoggerFactory createLazy(bool verbose, bool toLogFile = false);
  };
}
