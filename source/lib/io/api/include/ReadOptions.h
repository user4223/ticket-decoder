#pragma once

#include <vector>
#include <optional>

namespace io::api
{

    struct ReaderOptions
    {
        std::optional<unsigned int> dpi = {};
        std::vector<unsigned int> pageIndexes = {};
        bool grayscale = true;

        unsigned int getDpi() const { return dpi.value_or(300); }
    };
}
