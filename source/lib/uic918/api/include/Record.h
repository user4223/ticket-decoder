#pragma once

#include <string>

#include "lib/utility/include/JsonBuilder.h"

namespace uic918::api
{

  using json = nlohmann::json;

  class Record
  {
    std::string id;
    std::string version;
    json jsonValue;

  public:
    Record(std::string i, std::string v, ::utility::JsonBuilder &&jb)
        : id(i),
          version(v),
          jsonValue(jb.build())
    {
    }

    Record(Record const &) = delete;
    Record &operator=(Record const &) = delete;

    Record(Record &&) = default;
    Record &operator=(Record &&) = default;

    std::string const &getId() const
    {
      return id;
    }

    json getJson() const
    {
      return jsonValue;
    }
  };
}
