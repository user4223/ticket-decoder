#pragma once

#include <filesystem>
#include <vector>
#include <ostream>
#include <cstdint>

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

    /* Reads data from file pointed by given path and returns raw bytes as a sequence
     */
    std::vector<std::uint8_t> readBinary(std::filesystem::path filePath);

    /* Returns true when all given paths are directories
     */
    bool areDirectories(std::vector<std::filesystem::path> const &paths);

    /* Returns true when all given paths are regular files
     */
    bool areFiles(std::vector<std::filesystem::path> const &paths);

    void checkAndEnsureCompatiblePaths(std::filesystem::path const inputPath, std::filesystem::path outputPath);

}
