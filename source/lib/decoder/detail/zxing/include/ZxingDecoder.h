#pragma once

#include "lib/decoder/api/include/Decoder.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/DebugController.h"

#include <opencv2/core.hpp>

#include <memory>

namespace decoder::detail
{
  class ZxingDecoder : public api::Decoder
  {
    ::utility::DebugController &debugController;
    struct Internal;
    std::shared_ptr<Internal> internal;

  public:
    ZxingDecoder(infrastructure::Context &context, api::DecoderOptions defaultOptions);

    virtual api::Result decode(detector::api::Descriptor const &descriptor) override;

    virtual api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image) override;
  };
}
