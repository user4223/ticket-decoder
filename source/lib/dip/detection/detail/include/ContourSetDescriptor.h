#pragma once

#include "../../api/include/ContourDescriptor.h"

#include <vector>
#include <functional>
#include <optional>

struct ContourSetDescriptor
{

  unsigned int stepCount = 0;
  std::vector<ContourDescriptor> contours;
  std::optional<std::vector<ContourDescriptor>> debugContours;

  static ContourSetDescriptor fromContours(std::vector<ContourDescriptor> &&contours);

  static ContourSetDescriptor fromContours(std::vector<ContourDescriptor::ContourType> &&contours);

  void forEachContour(std::function<void(ContourDescriptor &)> &&consumer)
  {
    std::for_each(contours.begin(), contours.end(), consumer);
  }
};
