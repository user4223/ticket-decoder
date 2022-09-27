#pragma once

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>
#include <optional>

namespace dip::utility
{

  struct Source
  {
    std::optional<std::filesystem::path> path;
    std::optional<std::string> annotation;
    cv::Mat image;

    bool isValid() const
    {
      return !image.empty();
    }
  };
  class ImageSource
  {
    std::vector<std::filesystem::path> imagePaths;
    unsigned int inputSourceIndex = 0u;

    ImageSource(std::filesystem::path directory, unsigned int defaultSource);

  public:
    std::string nextSource();

    std::string previousSource();

    Source getSource() const;

    static ImageSource create(std::filesystem::path directory, unsigned int defaultSource);
  };
}
