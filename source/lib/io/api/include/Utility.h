#pragma once

#include <filesystem>
#include <vector>
#include <optional>

namespace io::api::utility
{

    /* Returns true when all given paths are directories
     */
    bool areDirectories(std::vector<std::filesystem::path> const &paths);

    /* Returns true when all given paths are regular files
     */
    bool areFiles(std::vector<std::filesystem::path> const &paths);

    void checkAndEnsureInputOutputPaths(std::filesystem::path const inputPath, std::optional<std::filesystem::path> optionalOutputPath);

}
