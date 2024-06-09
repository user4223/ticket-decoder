#pragma once

#include "lib/dip/detection/api/include/Descriptor.h"

#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include "Level.h"
#include "Result.h"

#include <memory>

namespace barcode::api
{

  struct DecoderOptions
  {
    bool pure = false;
    bool binarize = true;

    static DecoderOptions const DEFAULT;
  };

  class Decoder
  {
  public:
    virtual ~Decoder() = default;

    virtual api::Result decode(dip::detection::api::Descriptor const &descriptor) = 0;

    virtual api::Result decode(DecoderOptions options, dip::detection::api::Descriptor const &descriptor) = 0;

    virtual api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image) = 0;

    virtual api::Result decode(DecoderOptions options, unsigned int id, cv::Rect const &box, cv::Mat const &image) = 0;

    static std::unique_ptr<Decoder> create(::utility::LoggerFactory &loggerFactory, DecoderOptions defaultOptions = {});
  };
}
