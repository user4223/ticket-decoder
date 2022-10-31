#pragma once

#include "lib/dip/detection/api/include/Descriptor.h"

#include "lib/utility/include/LoggingFwd.h"

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

    static api::Result decode(::utility::LoggerFactory &loggerFactory, dip::detection::api::Descriptor const &descriptor, bool const pure);

    static api::Result decode(::utility::LoggerFactory &loggerFactory, unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure);
  };
}
