#pragma once

#include "Level.h"

#include <opencv2/core.hpp>

#include <vector>
#include <ostream>

namespace barcode::api
{
  struct Result
  {
    unsigned int id;
    cv::Rect box;
    Level level = Level::Unknown;
    std::vector<std::uint8_t> payload;

    Result(unsigned int id, cv::Rect const &box);

    bool isDecoded() const;

    cv::Mat visualize(cv::Mat &&image) const;

    static Result visualize(Result &&result, std::ostream &stream);
  };
}
