#pragma once

#include "Level.h"
#include "Result.h"

#include <vector>
#include <tuple>
#include <cstdint>

namespace barcode::api
{
  class Decoder
  {
  public:
    virtual ~Decoder() = default;

    virtual Level detect() = 0;

    virtual Result decode() = 0;

    static std::unique_ptr<Decoder> create(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure);

    static api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure);
  };
}
