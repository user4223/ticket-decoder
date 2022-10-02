#include "../include/Utility.h"

#include "lib/dip/filtering/include/Transform.h"
#include "lib/dip/utility/include/Text.h"

#include <opencv2/imgproc.hpp>

namespace dip::detection::api
{
  void visualize(cv::Mat &destination, cv::Mat const &source, cv::Rect const &box)
  {
    if (source.empty() || box == cv::Rect{})
    {
      return;
    }

    auto const &part = source.channels() == 3 ? source : dip::filtering::toColor(source.clone());
    part(cv::Rect(0, 0, box.width, box.height)).copyTo(destination(box));
  }
}
