// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/input/detail/api/include/Reader.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/infrastructure/include/Logger.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace input::detail
{

  class PdfReader : public Reader
  {
    infrastructure::Logger logger;
    struct Internal;

    std::shared_ptr<Internal> internal; // shared to make forward decl type possible

  public:
    PdfReader(infrastructure::Context &context, api::LoadOptions options);

    std::vector<std::string> supportedExtensions() const override;

    ReadResult read(std::filesystem::path path) const override;

    static std::vector<unsigned int> selectedPages(std::vector<unsigned int> const &pageIndexes, unsigned int pageCount);
  };
}
