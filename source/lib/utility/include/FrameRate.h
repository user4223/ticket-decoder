// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/infrastructure/include/ParameterSupplier.h"

#include <chrono>

namespace utility
{
    class FrameRate : public infrastructure::ParameterSupplier
    {
        unsigned int counter = 0u;
        unsigned int fps = 0u;
        std::chrono::time_point<std::chrono::steady_clock> previous;
        std::chrono::time_point<std::chrono::steady_clock> next;

    public:
        FrameRate();

        void update();

        unsigned int get() const;

        ParameterTypeList supplyParameters() const;
    };
}
