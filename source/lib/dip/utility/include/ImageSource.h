#pragma once

#include <filesystem>
#include <vector>
#include <optional>

namespace dip::utility
{
  class ImageSource
  {
    std::vector<std::filesystem::path> imagePaths;
    unsigned int inputSourceIndex = 0u;

    ImageSource(std::filesystem::path directory, unsigned int defaultSource);

  public:
    std::string nextSource();

    std::string previousSource();

    std::optional<std::filesystem::path> getSource() const;

    static ImageSource create(std::filesystem::path directory, unsigned int defaultSource);
  };
}
