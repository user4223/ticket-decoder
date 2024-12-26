#pragma once

#include "nlohmann/json.hpp"

#include <string>
#include <functional>
#include <optional>

namespace utility
{

  struct JsonBuilder
  {
  private:
    using json = nlohmann::json;

    json value;

    JsonBuilder(json &&v) : value(std::move(v)) {}

  public:
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
      return field != nullptr ? add(name, *field) : *this;
    }

    template <typename K>
    JsonBuilder &add(std::string name, std::optional<K> const &value)
    {
      return value.has_value() ? add(name, *value) : *this;
    }

    template <typename T>
    JsonBuilder &add(std::string name, T const &field)
    {
      value[name] = json(field);
      return *this;
    }

    template <typename T>
    JsonBuilder &add(T *const field)
    {
      return field != nullptr ? add(*field) : *this;
    }

    template <typename K>
    JsonBuilder &add(std::optional<K> const &value)
    {
      return value.has_value() ? add(*value) : *this;
    }

    template <typename T>
    JsonBuilder &add(T const &field)
    {
      value.insert(value.end(), json(field));
      return *this;
    }

    std::string build(int indent = -1)
    {
      return value.dump(indent);
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

  JsonBuilder toObject(unsigned int size, std::function<std::tuple<std::string, JsonBuilder>()> producer);

  JsonBuilder toArray(unsigned int size, std::function<JsonBuilder()> producer);

}
