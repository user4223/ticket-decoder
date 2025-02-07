
#include "../include/KeyMapper.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include <opencv2/highgui.hpp>

namespace ui
{
  KeyMapper::KeyMapper(infrastructure::Context &context, MappingType &&m) : KeyMapper(context, 1, std::move(m)) {}

  KeyMapper::KeyMapper(infrastructure::Context &context, int d, MappingType &&m)
      : logger(CREATE_LOGGER(context.getLoggerFactory())), delay(d), mappings(std::move(m))
  {
    auto terminator = [this]()
    { this->quit = true; return ""; };

    add({{27, terminator}, {'q', terminator}, {'Q', terminator}});
  }

  void KeyMapper::add(MappingType &&m)
  {
    mappings.insert(m.begin(), m.end());
  }

  std::tuple<bool, std::string> KeyMapper::handleInternal(int8_t key) const
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

  bool KeyMapper::handle(char key) const
  {
    auto const [success, message] = handleInternal(key);
    if (success)
    {
      LOG_INFO(logger) << "Handling key '" << key << "': " << message;
    }
    return success;
  }

  void KeyMapper::handle(std::function<void(bool)> handler) const
  {
    handler(true);
    for (int key = cv::waitKey(delay); !quit; key = cv::waitKey(delay))
    {
      handler(handle(key));
    }
  }
}
