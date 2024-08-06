#pragma once

#include "DecoderOptions.h"

#include "lib/dip/detection/api/include/Descriptor.h"

#include "lib/utility/include/LoggingFwd.h"
#include "lib/utility/include/DebugController.h"

#include <opencv2/core.hpp>

#include "Level.h"
#include "Result.h"

#include <memory>

namespace barcode::api
{
  class Decoder
  {
  public:
    virtual ~Decoder() = default;

    virtual api::Result decode(dip::detection::api::Descriptor const &descriptor) = 0;

    virtual api::Result decode(DecoderOptions options, dip::detection::api::Descriptor const &descriptor) = 0;

    virtual api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image) = 0;

    virtual api::Result decode(DecoderOptions options, unsigned int id, cv::Rect const &box, cv::Mat const &image) = 0;

    static std::unique_ptr<Decoder> create(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &debugController, DecoderOptions defaultOptions = {});
  };
}
