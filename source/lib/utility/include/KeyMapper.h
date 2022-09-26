#pragma once

#include <functional>
#include <map>
#include <string>
#include <tuple>

namespace utility
{
  class KeyMapper
  {
    volatile bool quit = false;
    int delay;
    using MappingType = std::map<char, std::function<std::string()>>;
    MappingType mappings;

  public:
    KeyMapper(int delay, MappingType &&mappings);
    KeyMapper(MappingType &&mappings);

    void add(MappingType &&mappings);

    std::tuple<bool, std::string> handle(char key) const;

    bool handle(std::ostream &stream, char key) const;

    void handle(std::ostream &stream, std::function<void(bool)> handler) const;
  };
}
