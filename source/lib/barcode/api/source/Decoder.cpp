
#include "../include/Decoder.h"

#include "../../detail/include/AztecDecoder.h"

namespace barcode::api
{
  std::unique_ptr<Decoder> Decoder::create(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure)
  {
    return std::unique_ptr<Decoder>{new detail::AztecDecoder(id, box, image, pure)};
  }

  Result Decoder::decode(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure)
  {
    return create(id, box, image, pure)->decode();
  }
}
