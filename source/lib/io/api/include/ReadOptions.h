#pragma once

#include <vector>
#include <optional>

namespace io::api
{

    struct ReadOptions
    {
        std::optional<unsigned int> dpi;
        std::vector<unsigned int> pageIndexes;

        unsigned int getDpi() const { return dpi.value_or(300); }
    };
}
