#pragma once

#include "ContourDetector.h"

#include <opencv2/core.hpp>

#include <memory>
#include <vector>

class ContourDetector
{
public:
  using FilterType = std::function<std::vector<ContourDescriptor>(std::vector<ContourDescriptor> &&)>;

  std::vector<ContourDescriptor> process(std::vector<ContourDescriptor> &&descriptors, std::vector<FilterType> &&filters) const;
};