#pragma once

#include "nlohmann/json.hpp"

#include <string>
#include <functional>
#include <optional>

namespace utility
{

  struct JsonBuilder
  {
    using json = nlohmann::json;

    json value;

    static JsonBuilder object()
    {
      return JsonBuilder{json::object()};
    }

    template <typename T>
    JsonBuilder &add(std::string name, T *const field)
    {
      if (field != nullptr)
      {
        add(name, *field);
      }
      return *this;
    }

    template <typename K>
    JsonBuilder &add(std::string name, std::optional<K> const &value)
    {
      if (value.has_value())
      {
        add(name, *value);
      }
      return *this;
    }

    template <typename T>
    JsonBuilder &add(std::string name, T const &field)
    {
      value[name] = json(field);
      return *this;
    }

    std::string build()
    {
      return value.dump();
    }
  };

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
