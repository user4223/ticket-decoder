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
    /* Pass __FILE__ here in normal cases to unify logger creation
     */
    Logger fromPath(std::string fullPath);

    static LoggerFactory create(bool verbose);

    static LoggerFactory createLazy(bool verbose);
  };
}
