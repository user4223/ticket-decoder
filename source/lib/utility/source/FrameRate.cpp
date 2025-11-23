// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/FrameRate.h"

namespace utility
{
    FrameRate::FrameRate()
        : previous(std::chrono::steady_clock::now()), next()
    {
    }

    void FrameRate::update()
    {
        counter += 1;
        next = std::chrono::steady_clock::now();
        if (next - previous >= std::chrono::seconds{1})
        {
            fps = counter;
            counter = 0;
            previous = next;
        }
    }

    unsigned int FrameRate::get() const
    {
        return fps;
    }

    FrameRate::ParameterTypeList FrameRate::supplyParameters() const
    {
        return {std::make_pair("fps:", std::to_string(fps))};
    }
}