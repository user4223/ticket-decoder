// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

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

  json toObject(unsigned int size, std::function<std::string(JsonBuilder &)> producer)
  {
    auto builder = JsonBuilder::object();
    for (auto index = 0; index < size; ++index)
    {
      auto object = JsonBuilder::object();
      auto name = producer(object);
      builder.add(name, std::move(object));
    }
    return builder.build();
  }

  json toArray(unsigned int size, std::function<void(JsonBuilder &)> producer)
  {
    auto builder = JsonBuilder::array();
    for (auto index = 0; index < size; ++index)
    {
      auto element = JsonBuilder::object();
      producer(element);
      builder.add(element.build());
    }
    return builder.build();
  }

  json toDynamicArray(unsigned int size, std::function<std::size_t(JsonBuilder &)> producer)
  {
    auto builder = JsonBuilder::array();
    for (auto consumedSize = 0; consumedSize < size;)
    {
      auto objectBuilder = JsonBuilder::object();
      consumedSize += producer(objectBuilder);
      builder.add(objectBuilder.build());
    }
    return builder.build();
  }
}
