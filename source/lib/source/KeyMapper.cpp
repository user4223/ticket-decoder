
#include "../include/KeyMapper.h"

KeyMapper::KeyMapper(MappingType &&m) : mappings(std::move(m)) {}

void KeyMapper::add(MappingType &&m)
{
  mappings.insert(m.begin(), m.end());
}

bool KeyMapper::handle(char key)
{
  auto const entry = mappings.find(key);
  if (entry == std::end(mappings))
  {
    return false;
  }

  entry->second();
  return true;
}