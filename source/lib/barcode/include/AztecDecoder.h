#pragma once

#include "Decoder.h"

#include <opencv2/core/mat.hpp>

#include <memory>
#include <vector>
#include <tuple>

namespace barcode::detail
{
  class AztecDecoder : public api::Decoder
  {
  private:
    struct Internal;

    std::shared_ptr<Internal> internal; // shared to make forward decl type possible

  public:
    AztecDecoder(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure);

    virtual api::Level detect() override;

    virtual api::Result decode() override;
  };
}
