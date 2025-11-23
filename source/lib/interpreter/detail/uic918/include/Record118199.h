// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

namespace interpreter::detail::uic
{
  class Record118199 : public AbstractRecord
  {
  public:
    Record118199(::utility::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual common::Context interpret(common::Context &&context) override;
  };
}
