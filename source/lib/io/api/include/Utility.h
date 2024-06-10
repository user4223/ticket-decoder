#pragma once

#include <filesystem>
#include <vector>
#include <optional>
#include <ostream>

namespace io::api::utility
{
    class OutputStream
    {
        struct Internal;
        std::shared_ptr<Internal> internal;
        std::ostream &stream;

    public:
        OutputStream(std::filesystem::path filePath);

        OutputStream();

        std::ostream &get() { return stream; }
    };

    /* Returns true when all given paths are directories
     */
    bool areDirectories(std::vector<std::filesystem::path> const &paths);

    /* Returns true when all given paths are regular files
     */
    bool areFiles(std::vector<std::filesystem::path> const &paths);

    void checkAndEnsureInputOutputPaths(std::filesystem::path const inputPath, std::optional<std::filesystem::path> optionalOutputPath);

}
