
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

  JsonBuilder toObject(unsigned int size, std::function<std::tuple<std::string, JsonBuilder>()> producer)
  {
    auto builder = JsonBuilder::object();
    for (auto index = 0; index < size; ++index)
    {
      auto [name, json] = producer();
      builder.add(name, std::move(json));
    }
    return builder;
  }

  JsonBuilder toArray(unsigned int size, std::function<JsonBuilder()> producer)
  {
    auto builder = JsonBuilder::array();
    for (auto index = 0; index < size; ++index)
    {
      builder.add(producer());
    }
    return builder;
  }

}
