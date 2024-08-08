#pragma once

#include "Level.h"

#include <opencv2/core.hpp>

#include <vector>
#include <cstdint>

namespace barcode::api
{
  struct Result
  {
    unsigned int id;
    cv::Rect box;
    cv::Mat image;
    Level level = Level::Unknown;
    std::vector<std::uint8_t> payload;

    Result(unsigned int id, cv::Rect const &box, cv::Mat const &image);

    Result(Result &&) = default;
    Result &operator=(Result &&) = default;

    Result(Result const &) = delete;
    Result &operator=(Result const &) = delete;

    bool isDecoded() const;
  };
}
