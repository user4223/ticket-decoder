// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <vector>
#include <cstdint>

namespace utility
{
    /* Reads data from file pointed by given path and returns raw bytes as a sequence
     */
    std::vector<std::uint8_t> readBinary(std::filesystem::path filePath);

    /* Returns true when all given paths are directories
     */
    bool areDirectories(std::vector<std::filesystem::path> const &paths);

    /* Returns true when all given paths are regular files
     */
    bool areFiles(std::vector<std::filesystem::path> const &paths);

    void ensureCompatiblePaths(std::filesystem::path const inputPath, std::filesystem::path outputPath);

    std::string normalizeExtension(std::filesystem::path const &path);

    bool isFilePath(std::filesystem::path const &path);
}
