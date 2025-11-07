#pragma once

namespace dip::detection::api
{
    /* Keep NOP-forward-detector the first because other detectors are optional
     */
    enum class DetectorType
    {
        NOP_FORWARDER,
        SQUARE_DETECTOR,
        CLASSIFIER,
    };

    DetectorType fromInt(unsigned int type);

    unsigned int toInt(DetectorType type);
}
