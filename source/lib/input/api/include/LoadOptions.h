#pragma once

#include <vector>

namespace io::api
{
    struct LoadOptions
    {
        unsigned int dpi = 300;
        std::vector<unsigned int> pageIndexes = {};
        bool grayscale = true;

        static LoadOptions const DEFAULT;
    };
}
