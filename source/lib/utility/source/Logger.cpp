
#include "../include/Logger.h"

namespace utility
{
    Logger::Logger(std::string n)
        : name(n)
    {
    }

    char const *Logger::getName() const
    {
        return name.c_str();
    }
}
