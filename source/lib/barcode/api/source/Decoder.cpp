
#include "../include/Decoder.h"

#include "../../detail/include/AztecDecoder.h"

namespace barcode::api
{
  std::unique_ptr<Decoder> Decoder::create(::utility::LoggerFactory &loggerFactory, DecoderConfig config)
  {
    return std::unique_ptr<Decoder>(new detail::AztecDecoder(loggerFactory, std::move(config)));
  }
}
