// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/PreProcessorOptions.h"

namespace dip
{

  PreProcessorOptions const PreProcessorOptions::DEFAULT = PreProcessorOptions{};

  OptionsBuilder::OptionsBuilder(PreProcessorOptions o)
      : options(std::move(o))
  {
  }

  PreProcessorOptions OptionsBuilder::build()
  {
    return std::move(options);
  }

  OptionsBuilder PreProcessorOptions::create()
  {
    return OptionsBuilder(DEFAULT);
  }
}
