#pragma once

#include "BarcodeDecoder.h"

#include "lib/dip/detection/api/include/Descriptor.h"

#include <opencv2/core/mat.hpp>

#include <memory>
#include <vector>
#include <tuple>

namespace barcode
{
  class AztecDecoder : public BarcodeDecoder
  {
  public:
    struct Internal;

  private:
    std::shared_ptr<Internal> internal; // shared to make forward decl type possible

    AztecDecoder(std::shared_ptr<Internal> internal);

  public:
    virtual BarcodeDecodingLevel detect() override;

    virtual BarcodeDecodingResult decode() override;

    static std::unique_ptr<BarcodeDecoder> create(dip::detection::api::Descriptor const &contourDescriptor, bool const pure);

    static BarcodeDecodingResult decode(dip::detection::api::Descriptor const &contourDescriptor, bool const pure);
  };
}
