#pragma once

#include "nlohmann/json.hpp"

#include <string>
#include <optional>
#include <algorithm>
#include <variant>

namespace utility
{

  using json = nlohmann::json;
  using keyType = std::variant<unsigned int, std::string>;

  template <typename... T>
  std::optional<std::string> getString(json const &node, std::convertible_to<keyType> auto &&...fields)
  {
    auto list = std::vector<keyType>();
    (list.push_back(std::forward<keyType>(fields)), ...);

    auto result = std::accumulate(
        std::begin(list), std::end(list), std::make_optional(node),
        [](std::optional<json> &&node, keyType const &key) -> std::optional<json>
        {
          if (!node)
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

          auto const* index = std::get_if<unsigned int>(&key);
          if (node->is_array() && index != nullptr)
          {
            if (node->size() > *index)
            {
              return std::make_optional(node->at(*index));
            }
            return std::nullopt;
          }

          return std::nullopt; });

    return (result && !result->is_null() && result->is_string())
               ? result
               : std::nullopt;
  }
}
