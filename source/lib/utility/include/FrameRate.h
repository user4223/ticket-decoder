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
