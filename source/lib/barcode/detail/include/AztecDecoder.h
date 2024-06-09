#pragma once

#include "../../api/include/Decoder.h"

#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <memory>

namespace barcode::detail
{
  class AztecDecoder : public api::Decoder
  {
    struct Internal;
    std::shared_ptr<Internal> internal;

  public:
    AztecDecoder(::utility::LoggerFactory &loggerFactory, api::DecoderOptions defaultOptions);

    virtual api::Result decode(dip::detection::api::Descriptor const &descriptor) override;

    virtual api::Result decode(api::DecoderOptions options, dip::detection::api::Descriptor const &descriptor) override;

    virtual api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image) override;

    virtual api::Result decode(api::DecoderOptions options, unsigned int id, cv::Rect const &box, cv::Mat const &image) override;
  };
}
