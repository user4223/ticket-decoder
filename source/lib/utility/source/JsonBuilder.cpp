
#include "../include/JsonBuilder.h"

namespace utility
{

  template <>
  JsonBuilder &JsonBuilder::add(std::string name, json const &subTree)
  {
    if (!subTree.empty())
    {
      value[name] = subTree;
    }
    return *this;
  }

  template <>
  JsonBuilder &JsonBuilder::add(std::string name, JsonBuilder const &subBuilder)
  {
    return add(name, subBuilder.value);
  }

}
