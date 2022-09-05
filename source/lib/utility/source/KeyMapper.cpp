
#include "../include/KeyMapper.h"

#include <opencv2/highgui.hpp>

#include <iostream>

namespace utility
{
  KeyMapper::KeyMapper(MappingType &&m)
      : mappings(std::move(m)) {}

  void KeyMapper::add(MappingType &&m)
  {
    mappings.insert(m.begin(), m.end());
  }

  std::tuple<bool, std::string> KeyMapper::handle(char key)
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

  bool KeyMapper::handle(char key, std::ostream &stream)
  {
    auto const [success, message] = handle(key);
    if (success)
    {
      stream << message << std::endl;
    }
    return success;
  }

  void KeyMapper::handle(std::ostream &stream, std::function<void()> handler)
  {
    auto quit = false;
    add({{27, [&]()
          { quit = true; return ""; }}});
    for (int key = cv::waitKey(10); !quit; key = cv::waitKey(10))
    {
      handle(key, std::cout);
      handler();
    }
  }
}
