#pragma once

#include <string>

namespace utility
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
