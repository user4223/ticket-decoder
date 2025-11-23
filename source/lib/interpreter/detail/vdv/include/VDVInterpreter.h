// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "lib/utility/include/Logger.h"

namespace interpreter::detail::vdv
{
  class VDVInterpreter : public common::Interpreter
  {
    ::utility::Logger logger;

  public:
    static TypeIdType getTypeId();

    VDVInterpreter(::utility::LoggerFactory &loggerFactory);

    virtual common::Context interpret(common::Context &&context) override;
  };
}
