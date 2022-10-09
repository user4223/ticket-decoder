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
    std::map<unsigned int, unsigned int> partMap;
    std::tuple<unsigned int, unsigned int> parts;
    int rotationDegree;
    unsigned int scaleFactor;

    ImageSource(std::filesystem::path directory, unsigned int defaultSource);

    void update();

  public:
    std::string nextSource();

    std::string previousSource();

    std::string rotateClockwise();

    std::string rotateCounterClockwise();

    std::string partReset();

    std::string togglePart2();

    std::string togglePart4();

    std::string upScale();

    std::string downScale();

    Source getSource() const;

    template <typename IteratorT>
    void to_string(IteratorT inserter)
    {
      *(inserter++) = std::make_pair("source:", annotation);
      *(inserter++) = std::make_pair("split:", std::to_string(std::get<0>(parts)) + "/" + std::to_string(std::get<1>(parts)));
      *(inserter++) = std::make_pair("rotation:", std::to_string(rotationDegree));
      *(inserter++) = std::make_pair("scale:", std::to_string(scaleFactor));
    }

    static ImageSource create(std::filesystem::path directory, unsigned int defaultSource);
  };
}
