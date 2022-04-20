
#include "../include/KeyMapper.h"

#include <iostream>

KeyMapper::KeyMapper(MappingType &&m) : mappings(std::move(m)) {}

void KeyMapper::add(MappingType &&m)
{
  mappings.insert(m.begin(), m.end());
}

std::tuple<bool, std::string> KeyMapper::handle(char key)
{
  auto const entry = mappings.find(key);
  if (entry == std::end(mappings))
  {
    return {false, ""};
  }

  return {true, entry->second()};
}

bool KeyMapper::handle(char key, std::ostream &stream)
{
  auto const [success, message] = handle(key);
  if (success)
  {
    stream << message << std::endl;
  }
  return success;
}