#pragma once

namespace barcode::api
{
  struct DecoderOptions
  {
    bool pure = false;
    bool binarize = true;

    static DecoderOptions const DEFAULT;
  };
}
