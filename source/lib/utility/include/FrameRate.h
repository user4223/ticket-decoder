#pragma once

#include <chrono>
#include <map>
#include <string>

namespace utility
{
    class FrameRate
    {
        unsigned int counter = 0u;
        unsigned int fps = 0u;
        std::chrono::time_point<std::chrono::steady_clock> previous;
        std::chrono::time_point<std::chrono::steady_clock> next;

    public:
        FrameRate();

        void update();

        unsigned int get() const;

        std::pair<std::string, std::string> toString() const;
    };
}
