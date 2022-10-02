#pragma once

#include <string>

namespace uic918::api
{
  class Record
  {
    std::string id;
    std::string version;
    std::string json;

  public:
    Record(std::string i, std::string v, std::string &&j)
        : id(i),
          version(v),
          json(std::move(j))
    {
    }

    Record(Record &&) = default;
    Record(Record const &) = default;
    Record &operator=(Record &&) = delete;
    Record &operator=(Record const &) = delete;

    std::string const &getId() const
    {
      return id;
    }

    std::string const &getJson() const
    {
      return json;
    }
  };

}
