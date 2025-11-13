
#include "../include/NopDecoder.h"

namespace barcode::api
{

  api::Result NopDecoder::decode(dip::detection::api::Descriptor const &descriptor)
  {
    return api::Result();
  }

  api::Result NopDecoder::decode(unsigned int id, cv::Rect const &box, cv::Mat const &image)
  {
    return api::Result();
  }
}
