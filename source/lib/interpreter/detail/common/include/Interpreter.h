#pragma once

#include "Context.h"

namespace uic918::detail
{
  /* Internal interface in the sense of the interpreter pattern to
     have multiple different implemenations for records or barcode types.
   */
  struct Interpreter
  {
    virtual ~Interpreter() = default;

    using TypeIdType = std::vector<std::uint8_t>;

    virtual Context interpret(Context &&context) = 0;
  };
}
