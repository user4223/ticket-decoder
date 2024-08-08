#include "../include/FrameRate.h"

namespace utility
{
    FrameRate::FrameRate()
        : previous(std::chrono::steady_clock::now()), next()
    {
    }

    void FrameRate::update()
    {
        counter += 1;
        next = std::chrono::steady_clock::now();
        if (next - previous >= std::chrono::seconds{1})
        {
            fps = counter;
            counter = 0;
            previous = next;
        }
    }

    unsigned int FrameRate::get() const
    {
        return fps;
    }
}