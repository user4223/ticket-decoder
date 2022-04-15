#pragma once

#include <opencv2/core.hpp>

#include <string>
#include <tuple>

class Decoder
{
public:
  virtual ~Decoder() = default;

  virtual std::tuple<bool, std::string> detect(cv::Mat const &image) = 0;
};