#pragma once

#include <string>

namespace dip::detection::api
{
  struct Parameters
  {
    unsigned int imageProcessingDebugStep = 0; // disabled
    unsigned int contourDetectorDebugStep = 0; // disabled

    std::string toString()
    {
      return "imageProcessingDebugStep: " + std::to_string(imageProcessingDebugStep) + " " + //
             "contourDetectorDebugStep: " + std::to_string(contourDetectorDebugStep);
    }
  };
}
