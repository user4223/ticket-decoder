#pragma once

namespace dip::detection::api
{
    enum class DetectorType
    {
        SQUARE_DETECTOR,
        CLASSIFIER,
        NOP_FORWARDER,
    };

    DetectorType fromInt(unsigned int type);

    unsigned int toInt(DetectorType type);
}
