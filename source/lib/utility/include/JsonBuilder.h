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

    static JsonBuilder array()
    {
      return JsonBuilder{json::array()};
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

    template <typename T>
    JsonBuilder &add(T const &field)
    {
      value.insert(value.end(), json(field));
      return *this;
    }

    std::string build()
    {
      return value.dump();
    }
  };

  template <>
  JsonBuilder &JsonBuilder::add(std::string name, json const &subTree);

  template <>
  JsonBuilder &JsonBuilder::add(std::string name, JsonBuilder const &subBuilder);

  template <>
  JsonBuilder &JsonBuilder::add(json const &subTree);

  template <>
  JsonBuilder &JsonBuilder::add(JsonBuilder const &subBuilder);

}
