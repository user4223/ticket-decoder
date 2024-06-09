
#include "../include/Transform.h"

#include <opencv2/imgproc.hpp>

#include <stdexcept>

namespace dip::filtering
{

  cv::Mat toGray(cv::Mat const &input)
  {
    if (input.channels() == 1)
    {
      return input.clone();
    }

    cv::Mat output;
    cv::cvtColor(input, output, cv::COLOR_RGB2GRAY);
    return output;
  }

  cv::Mat toColor(cv::Mat &&input)
  {
    if (input.channels() == 3)
    {
      return std::move(input);
    }

    cv::Mat transformed;
    cv::cvtColor(input, transformed, cv::COLOR_GRAY2RGB);
    return transformed;
  }

  cv::Mat rotate(cv::Mat const &input, float angle)
  {
    if (angle == 0.f)
    {
      return input;
    }

    auto const center = cv::Point2f{(input.cols - 1) / 2.f, (input.rows - 1) / 2.f};
    auto const rotation = cv::getRotationMatrix2D(center, angle, 1.f);
    auto output = cv::Mat(input.size(), input.type());
    cv::warpAffine(input, output, rotation, input.size(), cv::INTER_AREA, 0, input.channels() == 1 ? cv::Scalar(255) : cv::Scalar(255, 255, 255));
    return output;
  }

  cv::Mat flipX(cv::Mat const &input)
  {
    auto output = cv::Mat(input.size(), input.type());
    cv::flip(input, output, 0);
    return output;
  }

  cv::Mat flipY(cv::Mat const &input)
  {
    auto output = cv::Mat(input.size(), input.type());
    cv::flip(input, output, 1);
    return output;
  }

  cv::Mat flipXY(cv::Mat const &input)
  {
    auto output = cv::Mat(input.size(), input.type());
    cv::flip(input, output, -1);
    return output;
  }

  cv::Mat scale(cv::Mat const &input, float scale)
  {
    auto output = cv::Mat();
    cv::resize(input, output, cv::Size{}, scale, scale, cv::INTER_AREA);
    return output;
  }

  cv::Rect split(cv::Size const &size, unsigned int partCount, unsigned int part)
  {
    if (part < 1 || part > partCount)
    {
      throw std::runtime_error(std::string("Part out of bounds: ") + std::to_string(part));
    }

    part -= 1;
    switch (partCount)
    {
    case 2:
    {
      auto const widthOverHeight = size.width >= size.height;
      auto const width = widthOverHeight ? size.width / 2 : size.width;
      auto const height = !widthOverHeight ? size.height / 2 : size.height;
      return cv::Rect(part * (widthOverHeight ? width : 0), part * (widthOverHeight ? 0 : height), width, height);
    }
    break;
    case 4:
    {
      auto const width = size.width / 2;
      auto const height = size.height / 2;
      return cv::Rect(part == 0 || part == 2 ? 0 : width, part == 0 || part == 1 ? 0 : height, width, height);
    }
    break;
    default:
      break;
    }

    throw std::runtime_error(std::string("Number of parts unsupported: ") + std::to_string(partCount));
  }

  cv::Mat split(cv::Mat const &image, unsigned int partCount, unsigned int part)
  {
    if (part == 0)
    {
      return image;
    }

    return image(split(image.size(), partCount, part)).clone();
  }

  cv::Mat crazySquareBinarizer(cv::Mat const &image)
  {
    // TODO Try to implement binarizer taking square shape into account
    return image;
  }
}
