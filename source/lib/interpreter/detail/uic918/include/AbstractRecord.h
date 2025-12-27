// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/infrastructure/include/LoggingFwd.h"

#include "lib/interpreter/detail/common/include/Interpreter.h"

#include "RecordHeader.h"

namespace interpreter::detail::uic
{

  class AbstractRecord : public common::Interpreter
  {
  protected:
    RecordHeader header;
    infrastructure::Logger logger;

    AbstractRecord(infrastructure::Logger &&logger, RecordHeader &&header);
  };

}
