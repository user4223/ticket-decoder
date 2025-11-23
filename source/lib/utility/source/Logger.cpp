// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Logger.h"

namespace utility
{
    Logger::Logger(std::string n)
        : name(n)
    {
    }

    char const *Logger::getName() const
    {
        return name.c_str();
    }
}
