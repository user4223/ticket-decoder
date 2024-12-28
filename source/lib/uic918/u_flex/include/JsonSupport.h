#pragma once

#include "lib/utility/include/JsonBuilder.h"

namespace utility
{
  template <typename ElementT, typename T>
    requires(!std::is_pointer_v<T>)
  static json toArray(T const &source, std::function<void(ElementT const &, JsonBuilder &)> transformer)
  {
    auto builder = JsonBuilder::array();
    for (auto index = 0; index < source.list.count; ++index)
    {
      auto object = JsonBuilder::object();
      transformer(*(source.list.array[index]), object);
      builder.add(object);
    }
    return builder.build();
  }

  template <typename ElementT, typename T>
  static json toArray(T const *const source, std::function<void(ElementT const &, JsonBuilder &)> transformer)
  {
    if (source == nullptr)
    {
      return json::array();
    }
    return toArray<ElementT, T>(*source, transformer);
  }

  template <typename ElementT>
    requires(!std::is_pointer_v<ElementT>)
  static json toArray(ElementT const &source)
  {
    auto builder = JsonBuilder::array();
    for (auto index = 0; index < source.list.count; ++index)
    {
      builder.add(source.list.array[index]);
    }
    return builder.build();
  }

  template <typename ElementT>
  static json toArray(ElementT const *const source)
  {
    if (source == nullptr)
    {
      return json::array();
    }
    return toArray(*source);
  }

  template <typename ElementT>
    requires(!std::is_pointer_v<ElementT>)
  static json toObject(ElementT const &source, std::function<void(ElementT const &, JsonBuilder &)> transformer)
  {
    auto builder = JsonBuilder::object();
    transformer(source, builder);
    return builder.build();
  }

  template <typename ElementT>
  static json toObject(ElementT const *const source, std::function<void(ElementT const &, JsonBuilder &)> transformer)
  {
    if (source == nullptr)
    {
      return json::object();
    }
    return toObject(*source, transformer);
  }
}
