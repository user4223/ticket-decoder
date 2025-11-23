// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Decoder.h"
#include "../include/NopDecoder.h"

#ifdef WITH_BARCODE_DECODER
#include "lib/decoder/detail/zxing/include/ZxingDecoder.h"
#endif

namespace decoder::api
{
  std::unique_ptr<Decoder> Decoder::create(infrastructure::Context &context, DecoderOptions defaultOptions)
  {
#ifdef WITH_BARCODE_DECODER
    return std::unique_ptr<Decoder>(new detail::ZxingDecoder(context, std::move(defaultOptions)));
#else
    return std::unique_ptr<Decoder>(new NopDecoder());
#endif
  }
}
