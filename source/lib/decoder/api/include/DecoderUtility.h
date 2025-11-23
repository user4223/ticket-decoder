// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Result.h"

#include "lib/dip/include/ShapeProperties.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace decoder::api
{
  void dump(std::filesystem::path const &basePath, Result const &result);

  dip::ShapeProperties getDrawProperties(Level level);

  char getCharacter(Level level);
}
