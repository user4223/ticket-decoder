#pragma once

#include <functional>
#include <map>
#include <string>
#include <tuple>

namespace utility
{
  class KeyMapper
  {
    using MappingType = std::map<char, std::function<std::string()>>;
    MappingType mappings;

  public:
    KeyMapper(MappingType &&mappings);

    void add(MappingType &&mappings);

    std::tuple<bool, std::string> handle(char key);

    bool handle(char key, std::ostream &stream);

    void handle(std::ostream &stream, std::function<void()> handler);
  };
}
