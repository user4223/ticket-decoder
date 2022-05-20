#pragma once

#include "ContourDescriptor.h"

#include <vector>

struct ContourSetDescriptor {


  unsigned int stepCount = 0;
  std::vector<ContourDescriptor> contours;
};
