
#include "../include/Result.h"

#include "lib/dip/filtering/include/Transform.h"

#include <functional>

namespace dip::detection::api
{
  Result::Result(std::vector<Descriptor> &&d)
      : Result(std::move(d), std::optional<cv::Mat>{}, std::optional<std::vector<Descriptor>>{})
  {
  }

  Result::Result(std::vector<Descriptor> &&d, std::optional<cv::Mat> &&di, std::optional<std::vector<Descriptor>> dd)
      : contours(std::move(d)), debugImage(std::move(di)), debugContours(std::move(dd))
  {
  }

  size_t Result::for_each(std::function<void(Descriptor const &)> handler) const
  {
    std::for_each(contours.begin(), contours.end(), handler);
    return contours.size();
  }
}
