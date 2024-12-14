#pragma once

#include "nlohmann/json.hpp"

#include <string>
#include <optional>
#include <algorithm>

namespace utility
{

  using json = nlohmann::json;

  template <typename... T>
  std::optional<std::string> getString(json const &node, std::convertible_to<std::string> auto &&...fields)
  {
    auto list = std::vector<std::string>();
    (list.push_back(std::forward<std::string>(fields)), ...);

    auto result = std::accumulate(
        std::begin(list), std::end(list), std::make_optional(node),
        [](auto &&node, auto const &key) -> std::optional<json>
        {
          if (!node)
          {
            return node;
          }

          auto const item = node->find(key);
          if (item != node->end())
          {
            return std::make_optional(*item);
          }
          return std::nullopt; });

    return result;
  }
}
