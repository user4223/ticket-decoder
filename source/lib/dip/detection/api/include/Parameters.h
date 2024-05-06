#pragma once

#include <string>
#include <filesystem>

namespace dip::detection::api
{
  struct Parameters
  {
    std::filesystem::path executableFolderPath;
    unsigned int imageProcessingDebugStep = 0; // disabled
    unsigned int contourDetectorDebugStep = 0; // disabled

    std::string toString() const
    {
      return "imageProcessingDebugStep: " + std::to_string(imageProcessingDebugStep) + " " + //
             "contourDetectorDebugStep: " + std::to_string(contourDetectorDebugStep);
    }

    template <typename IteratorT>
    void toString(IteratorT inserter) const
    {
      *(inserter++) = std::make_pair(std::string("debug dip:"), std::to_string(imageProcessingDebugStep));
      *(inserter++) = std::make_pair(std::string("debug cd:"), std::to_string(contourDetectorDebugStep));
    }
  };
}
