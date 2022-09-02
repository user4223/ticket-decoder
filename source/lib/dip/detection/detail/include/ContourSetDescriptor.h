#pragma once

#include "../../api/include/Descriptor.h"

#include <vector>
#include <functional>
#include <optional>

namespace dip::detection::detail
{
  struct ContourSetDescriptor
  {

    unsigned int stepCount = 0;
    std::vector<api::Descriptor> contours;
    std::optional<std::vector<api::Descriptor>> debugContours;

    static ContourSetDescriptor fromContours(std::vector<api::Descriptor> &&contours);

    static ContourSetDescriptor fromContours(std::vector<api::Descriptor::ContourType> &&contours);

    void forEachContour(std::function<void(api::Descriptor &)> &&consumer)
    {
      std::for_each(contours.begin(), contours.end(), consumer);
    }
  };
}
