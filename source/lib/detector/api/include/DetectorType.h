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
