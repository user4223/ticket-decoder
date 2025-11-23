// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

namespace detector::api
{
    /* Keep NOP-forward-detector the first because other detectors are optional
     */
    enum class DetectorType
    {
        NOP_DETECTOR,
        SQUARE_DETECTOR,
        CLASSIFIER_DETECTOR,
    };

    DetectorType fromInt(unsigned int type);

    unsigned int toInt(DetectorType type);
}
