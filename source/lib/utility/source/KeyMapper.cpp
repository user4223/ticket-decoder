
#include "../include/KeyMapper.h"

#include <opencv2/highgui.hpp>

#include <iostream>

namespace utility
{
  KeyMapper::KeyMapper(MappingType &&m) : KeyMapper(1, std::move(m)) {}

  KeyMapper::KeyMapper(int d, MappingType &&m)
      : delay(d), mappings(std::move(m))
  {
    auto terminator = [this]()
    { this->quit = true; return ""; };

    add({{27, terminator}, {'q', terminator}, {'Q', terminator}});
  }

  void KeyMapper::add(MappingType &&m)
  {
    mappings.insert(m.begin(), m.end());
  }

  std::tuple<bool, std::string> KeyMapper::handle(char key) const
  {
    if (key == -1)
    {
      return {false, ""};
    }
    auto const entry = mappings.find(key);
    if (entry == std::end(mappings))
    {
      return {false, ""};
    }

    return {true, entry->second()};
  }

  bool KeyMapper::handle(char key, std::ostream &stream) const
  {
    auto const [success, message] = handle(key);
    if (success)
    {
      stream << message << std::endl;
    }
    return success;
  }

  void KeyMapper::handle(std::ostream &stream, std::function<void()> handler) const
  {
    for (int key = cv::waitKey(delay); !quit; key = cv::waitKey(delay))
    {
      handle(key, std::cout);
      handler();
    }
  }
}
