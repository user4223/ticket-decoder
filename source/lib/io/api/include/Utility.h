#pragma once

#include <filesystem>
#include <vector>
#include <optional>

namespace io::api::utility
{

    bool areDirectories(std::vector<std::filesystem::path> const &paths);

    bool areFiles(std::vector<std::filesystem::path> const &paths);

    void checkAndEnsureInputOutputPaths(std::filesystem::path const inputPath, std::optional<std::filesystem::path> optionalOutputPath);

}
