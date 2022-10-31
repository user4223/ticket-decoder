
#include "../include/Decoder.h"

#include "../../detail/include/AztecDecoder.h"

namespace barcode::api
{
  Result Decoder::decode(::utility::LoggerFactory &loggerFactory, dip::detection::api::Descriptor const &contourDescriptor, bool const pure)
  {
    return decode(loggerFactory, contourDescriptor.id, contourDescriptor.square, contourDescriptor.image, pure);
  }

  Result Decoder::decode(::utility::LoggerFactory &loggerFactory, unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure)
  {
    auto decoder = std::unique_ptr<Decoder>{new detail::AztecDecoder(loggerFactory, id, box, image, pure)};
    return decoder->decode();
  }
}
