#pragma once

#include "Logger.h"

#include <string>

namespace utility
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
