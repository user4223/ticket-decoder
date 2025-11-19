#pragma once

#include "Decoder.h"

namespace decoder::api
{
  class NopDecoder : public api::Decoder
  {
  public:
    virtual api::Result decode(detector::api::Descriptor const &descriptor);

    virtual api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image);
  };
}
