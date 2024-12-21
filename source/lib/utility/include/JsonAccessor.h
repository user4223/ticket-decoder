#pragma once

#include "nlohmann/json.hpp"

#include <string>
#include <optional>
#include <algorithm>
#include <variant>

namespace utility
{

  using json = nlohmann::json;
  using keyType = std::variant<int, std::string>;

  template <typename T>
  struct Consumer
  {
    std::optional<T> const value;

    bool operator()(std::function<void(T)> consumer) const
    {
      if (value)
      {
        consumer(*value);
        return true;
      }
      return false;
    }
  };

  std::optional<json> getNode(json const &node, std::convertible_to<keyType> auto &&...fields)
  {
    auto list = std::vector<keyType>();
    (list.push_back(std::forward<keyType>(fields)), ...);

    auto result = std::accumulate(std::begin(list), std::end(list), std::make_optional(node),
                                  [](std::optional<json> &&node, keyType const &key) -> std::optional<json>
                                  {
                                    if (!node || node->empty())
                                    {
                                      return node;
                                    }

                                    auto const* field = std::get_if<std::string>(&key);
                                    if (node->is_object() && field != nullptr)
                                    {
                                        auto const iterator = node->find(*field);
                                        if (iterator != node->end())
                                        {
                                          return std::make_optional(*iterator);
                                        }
                                        return std::nullopt;
                                    }

                                    auto const* index = std::get_if<int>(&key);
                                    if (node->is_array() && index != nullptr)
                                    {
                                        if (*index >= 0 && node->size() > *index)
                                        {
                                          return std::make_optional(node->at(*index));
                                        }
                                        return std::nullopt;
                                    }

                                    return std::nullopt; });

    return (result && !result->is_null())
               ? result
               : std::nullopt;
  }

  Consumer<json> ifNode(json const &node, std::convertible_to<keyType> auto &&...fields)
  {
    auto result = getNode(node, fields...);
    return result ? Consumer<json>{std::move(result)} : Consumer<json>{};
  }

  std::optional<std::string> getString(json const &node, std::convertible_to<keyType> auto &&...fields)
  {
    auto result = getNode(node, fields...);
    return result && result->is_string()
               ? std::optional(result->template get<std::string>())
               : std::nullopt;
  }

  Consumer<std::string> ifString(json const &node, std::convertible_to<keyType> auto &&...fields)
  {
    auto result = getString(node, fields...);
    return result ? Consumer<std::string>{std::move(result)} : Consumer<std::string>{};
  }

  std::optional<bool> getBool(json const &node, std::convertible_to<keyType> auto &&...fields)
  {
    auto result = getNode(node, fields...);
    return result && result->is_boolean()
               ? std::optional(result->template get<bool>())
               : std::nullopt;
  }

  Consumer<bool> ifBool(json const &node, std::convertible_to<keyType> auto &&...fields)
  {
    auto result = getBool(node, fields...);
    return result ? Consumer<bool>{std::move(result)} : Consumer<bool>{};
  }
}
