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
