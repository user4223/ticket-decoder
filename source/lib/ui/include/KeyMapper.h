// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include <functional>
#include <map>
#include <string>
#include <tuple>

namespace ui
{
  class KeyMapper
  {
  public:
    using MappingType = std::map<int8_t, std::function<std::string()>>;

  private:
    ::utility::Logger logger;
    volatile bool quit = false;
    int const delay;
    MappingType mappings;

    std::tuple<bool, std::string> handleInternal(int8_t key) const;

  public:
    KeyMapper(infrastructure::Context &context, int delay, MappingType &&mappings);
    KeyMapper(infrastructure::Context &context, MappingType &&mappings);

    void add(MappingType &&mappings);

    bool handle(char key) const;

    void handle(std::function<void(bool)> handler) const;
  };
}
