#pragma once

#include "lib/utility/include/LoggingFwd.h"

#include <functional>
#include <map>
#include <string>
#include <tuple>

namespace utility
{
  class KeyMapper
  {
    ::utility::Logger logger;
    volatile bool quit = false;
    int const delay;
    using MappingType = std::map<char, std::function<std::string()>>;
    MappingType mappings;

    std::tuple<bool, std::string> handleInternal(char key) const;

  public:
    KeyMapper(LoggerFactory &loggerFactory, int delay, MappingType &&mappings);
    KeyMapper(LoggerFactory &loggerFactory, MappingType &&mappings);

    void add(MappingType &&mappings);

    bool handle(char key) const;

    void handle(std::function<void(bool)> handler) const;
  };
}
