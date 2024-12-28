#pragma once

#include "nlohmann/json.hpp"

#include <string>
#include <functional>
#include <optional>

namespace utility
{

  using json = nlohmann::json;

  struct JsonBuilder
  {
  private:
    json value;

  public:
    JsonBuilder() = default;
    JsonBuilder(JsonBuilder &&) = default;
    JsonBuilder &operator=(JsonBuilder &&) = default;

    /* Copy not wanted */
    JsonBuilder(JsonBuilder const &) = delete;
    JsonBuilder &operator=(JsonBuilder const &) = delete;

    JsonBuilder(json &&v)
        : value(std::move(v))
    {
    }

    JsonBuilder(std::string jsonString)
        : value(json::parse(jsonString, nullptr, false))
    {
    }

    static JsonBuilder object()
    {
      return {json::object()};
    }

    static JsonBuilder array()
    {
      return {json::array()};
    }

    template <typename T>
    JsonBuilder &add(std::string name, T *const field)
    {
      return field != nullptr ? add(name, *field) : *this;
    }

    template <typename T>
    JsonBuilder &add(std::string name, T const &field)
    {
      value[name] = json(field);
      return *this;
    }

    JsonBuilder &add(std::string name, json &&field)
    {
      if (!field.empty())
      {
        value[name] = std::move(field);
      }
      return *this;
    }

    JsonBuilder &add(std::string name, JsonBuilder &&builder)
    {
      return add(name, builder.build());
    }

    template <typename K>
    JsonBuilder &add(std::string name, std::optional<K> const &value)
    {
      return value.has_value() ? add(name, *value) : *this;
    }

    template <typename T>
    JsonBuilder &add(T *const field)
    {
      return field != nullptr ? add(*field) : *this;
    }

    template <typename T>
    JsonBuilder &add(T const &field)
    {
      value.insert(value.end(), json(field));
      return *this;
    }

    JsonBuilder &add(json &&field)
    {
      if (!field.empty())
      {
        value.insert(value.end(), std::move(field));
      }
      return *this;
    }

    JsonBuilder &add(JsonBuilder &&builder)
    {
      return add(builder.build());
    }

    template <typename K>
    JsonBuilder &add(std::optional<K> const &value)
    {
      return value.has_value() ? add(*value) : *this;
    }

    std::string buildString(int indent = -1) const
    {
      return value.dump(indent);
    }

    json build()
    {
      return std::move(value);
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

  json toObject(unsigned int size, std::function<std::string(JsonBuilder &)> producer);

  json toArray(unsigned int size, std::function<void(JsonBuilder &)> producer);

  json toDynamicArray(unsigned int size, std::function<std::size_t(JsonBuilder &)> producer);
}
