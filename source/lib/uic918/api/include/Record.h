#pragma once

#include <string>

namespace uic918
{
  class Record
  {
    std::string const id;
    std::string const version;
    std::string const json;

  public:
    Record(std::string i, std::string v, std::string &&j)
        : id(i),
          version(v),
          json(std::move(j))
    {
    }

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
