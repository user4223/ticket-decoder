// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <optional>

namespace detector::api
{
  struct DetectorOptions
  {
    std::optional<std::filesystem::path> classifierFile;
  };
}
