
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

  template <>
  JsonBuilder &JsonBuilder::add(json const &subTree)
  {
    if (!subTree.empty())
    {
      value.insert(value.end(), subTree);
    }
    return *this;
  }

  template <>
  JsonBuilder &JsonBuilder::add(JsonBuilder const &subBuilder)
  {
    return add(subBuilder.value);
  }

}
