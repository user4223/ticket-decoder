#pragma once

#include <filesystem>

namespace test::support
{
    class TempPath
    {
        std::filesystem::path path;

    public:
        TempPath(bool changeCurrentDirectory = true);
        ~TempPath();

        std::filesystem::path get() const { return path; }
    };
}
