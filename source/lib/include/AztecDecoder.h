#pragma once

#include "Decoder.h"

#include <memory>

class AztecDecoder : public Decoder
{
public:
  static std::unique_ptr<Decoder> create();

  virtual std::tuple<bool, std::string> detect(cv::Mat const &image);
};