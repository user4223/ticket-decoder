// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Context.h"

namespace interpreter::detail::common
{
  /* Internal interface in the sense of the interpreter pattern to
     have multiple different implemenations for records or barcode types.
   */
  struct Interpreter
  {
    virtual ~Interpreter() = default;

    virtual bool canInterpret(Context const &context) const = 0;

    virtual Context interpret(Context &&context) = 0;
  };
}
