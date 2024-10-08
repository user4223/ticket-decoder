#pragma once

#include "../../api/include/Decoder.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/DebugController.h"

#include <opencv2/core.hpp>

#include <memory>

namespace barcode::detail
{
  class AztecDecoder : public api::Decoder
  {
    ::utility::DebugController &debugController;
    struct Internal;
    std::shared_ptr<Internal> internal;

  public:
    AztecDecoder(infrastructure::Context &context, api::DecoderOptions defaultOptions);

    virtual api::Result decode(dip::detection::api::Descriptor const &descriptor) override;

    virtual api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image) override;
  };
}
