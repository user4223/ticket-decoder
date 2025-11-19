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
