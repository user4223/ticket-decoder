#include "../include/Utility.h"

#include <algorithm>

namespace io::api::utility
{

    bool areDirectories(std::vector<std::filesystem::path> const &paths)
    {
        if (paths.empty())
        {
            return false;
        }
        return std::all_of(std::begin(paths), std::end(paths), [&](auto const &p)
                           { return std::filesystem::is_directory(p); });
    }

    bool areFiles(std::vector<std::filesystem::path> const &paths)
    {
        if (paths.empty())
        {
            return false;
        }
        return std::all_of(std::begin(paths), std::end(paths), [&](auto const &p)
                           { return std::filesystem::is_regular_file(p); });
    }

}
