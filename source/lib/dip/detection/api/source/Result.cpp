
#include "../include/Result.h"

#include "lib/dip/filtering/include/Transform.h"

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
}
