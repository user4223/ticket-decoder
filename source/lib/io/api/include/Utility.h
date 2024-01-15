#pragma once

#include <filesystem>
#include <vector>

namespace io::api::utility
{

    bool areDirectories(std::vector<std::filesystem::path> const &paths);

    bool areFiles(std::vector<std::filesystem::path> const &paths);

}
