
#include "../include/Decoder.h"

#include "../../detail/include/AztecDecoder.h"

namespace barcode::api
{
  std::unique_ptr<Decoder> Decoder::create(infrastructure::Context &context, DecoderOptions defaultOptions)
  {
    return std::unique_ptr<Decoder>(new detail::AztecDecoder(context, std::move(defaultOptions)));
  }
}
