// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/AbstractRecord.h"

namespace interpreter::detail::uic
{
  AbstractRecord::AbstractRecord(::utility::Logger &&l, RecordHeader &&h)
      : logger(std::move(l)), header(std::move(h))
  {
  }
}
