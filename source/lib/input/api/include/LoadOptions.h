// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <vector>

namespace input::api
{
    struct LoadOptions
    {
        unsigned int dpi = 300;
        std::vector<unsigned int> pageIndexes = {};
        bool grayscale = true;

        static LoadOptions const DEFAULT;
    };
}
