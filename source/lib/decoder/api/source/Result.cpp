
#include "../include/Result.h"

namespace decoder::api
{

  Result::Result(unsigned int id, cv::Rect const &b, cv::Mat const &i)
      : id(id), box(b), image(i)
  {
  }

  bool Result::isDecoded() const
  {
    return level == decoder::api::Level::Decoded && !payload.empty();
  }
}
