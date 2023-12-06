#pragma once

#include <vector>

namespace io::api
{

    struct ReadOptions
    {
        std::vector<unsigned int> pages;
    };

}