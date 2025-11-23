// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/DetectorType.h"

#include <type_traits>

namespace detector::api
{
    DetectorType fromInt(unsigned int type)
    {
        return static_cast<DetectorType>(type);
    }

    unsigned int toInt(DetectorType type)
    {
        return static_cast<std::underlying_type<DetectorType>::type>(type);
    }
}
