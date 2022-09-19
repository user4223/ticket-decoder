
#include "../include/Result.h"

namespace barcode::api
{

  Result::Result(unsigned int id, cv::Rect const &b, cv::Mat const &i)
      : id(id), box(b), image(i)
  {
  }

  bool Result::isDecoded() const
  {
    return level == barcode::api::Level::Decoded && !payload.empty();
  }
}
