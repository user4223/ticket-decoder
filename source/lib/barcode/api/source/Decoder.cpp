
#include "../include/Decoder.h"

#include "../../detail/include/AztecDecoder.h"

namespace barcode::api
{
  Result Decoder::decode(::utility::LoggerFactory &loggerFactory, dip::detection::api::Descriptor const &contourDescriptor, Config config)
  {
    return decode(loggerFactory, contourDescriptor.id, contourDescriptor.square, contourDescriptor.image, std::move(config));
  }

  Result Decoder::decode(::utility::LoggerFactory &loggerFactory, unsigned int id, cv::Rect const &box, cv::Mat const &image, Config config)
  {
    auto decoder = std::unique_ptr<Decoder>{new detail::AztecDecoder(loggerFactory, id, box, image, std::move(config))};
    return decoder->decode();
  }
}
