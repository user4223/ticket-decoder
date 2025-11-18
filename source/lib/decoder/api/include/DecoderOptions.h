#pragma once

namespace decoder::api
{
  struct DecoderOptions
  {
    bool pure = false;
    bool binarize = true;

    static DecoderOptions const DEFAULT;
  };
}
