
#include "../include/Decoder.h"

#include "../../detail/include/AztecDecoder.h"

namespace barcode::api
{
  DecoderOptions const DecoderOptions::DEFAULT = DecoderOptions{};

  std::unique_ptr<Decoder> Decoder::create(::utility::LoggerFactory &loggerFactory, DecoderOptions defaultOptions)
  {
    return std::unique_ptr<Decoder>(new detail::AztecDecoder(loggerFactory, std::move(defaultOptions)));
  }
}
