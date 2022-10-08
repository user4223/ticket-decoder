#pragma once

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>
#include <optional>
#include <map>

namespace dip::utility
{
  struct Source
  {
    std::optional<std::filesystem::path> path;
    std::string annotation;
    cv::Mat image;

    bool isValid() const
    {
      return !image.empty();
    }

    bool isCamera() const
    {
      return !path;
    }
  };
  class ImageSource
  {
    std::vector<std::filesystem::path> imagePaths;
    unsigned int inputSourceIndex = 0u;
    std::optional<std::filesystem::path> path;
    std::string annotation;
    int rotationDegree;
    std::map<unsigned int, unsigned int> parts;
    unsigned int scaleFactor;

    ImageSource(std::filesystem::path directory, unsigned int defaultSource);

    std::string updatePath();

  public:
    std::string nextSource();

    std::string previousSource();

    std::string rotateClockwise();

    std::string rotateCounterClockwise();

    std::string togglePart2();

    std::string togglePart4();

    std::string upScale();

    std::string downScale();

    Source getSource() const;

    template <typename IteratorT>
    void to_string(IteratorT inserter)
    {
      *(inserter++) = "source:    " + annotation;
      *(inserter++) = "split:     ";
      *(inserter++) = "rotation:  " + std::to_string(rotationDegree);
      *(inserter++) = "scale:     " + std::to_string(scaleFactor);
    }

    static ImageSource create(std::filesystem::path directory, unsigned int defaultSource);
  };
}
