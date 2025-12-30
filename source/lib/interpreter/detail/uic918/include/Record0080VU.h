// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "AbstractRecord.h"
#include "RecordHeader.h"

#include <optional>
#include <string>
#include <cstdint>

namespace interpreter::detail::uic
{
  class Record0080VU : public AbstractRecord
  {
  public:
    Record0080VU(infrastructure::LoggerFactory &loggerFactory, RecordHeader &&header);

    virtual common::Context interpret(common::Context &&context) override;

    static std::optional<std::string> getProduktbezeichnung(std::uint16_t const pvOrgId, std::uint16_t const pvProduktnummer);
  };
}
