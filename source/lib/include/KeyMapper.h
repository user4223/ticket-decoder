
#include <functional>
#include <map>

class KeyMapper
{
  using MappingType = std::map<char, std::function<void()>>;
  MappingType mappings;

public:
  KeyMapper(MappingType &&mappings);

  void add(MappingType &&mappings);

  bool handle(char key);
};