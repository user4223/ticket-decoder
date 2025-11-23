// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/FileSystem.h"

#include <algorithm>
#include <iostream>
#include <fstream>

namespace utility
{
    std::vector<std::uint8_t> readBinary(std::filesystem::path filePath)
    {
        if (!std::filesystem::is_regular_file(filePath))
        {
            throw std::runtime_error(std::string("Path to load raw data from does not exist or does not point to a file: ") + filePath.string());
        }

        auto ifs = std::ifstream(filePath, std::ios::binary | std::ios::ate);
        auto const size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        auto buffer = std::vector<std::uint8_t>(size);
        ifs.read(reinterpret_cast<char *>(buffer.data()), size);
        return buffer;
    }

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

    void ensureCompatiblePaths(std::filesystem::path const inputPath, std::filesystem::path const outputPath)
    {
        if (!std::filesystem::exists(inputPath))
        {
            throw std::invalid_argument("Input path does not exists: " + outputPath.string());
        }

        if (std::filesystem::is_directory(inputPath))
        {
            if (!std::filesystem::exists(outputPath))
            {
                if (isFilePath(outputPath))
                {
                    throw std::invalid_argument("Input path is a directory and given output path does not exists but looks like a file: " + outputPath.string());
                }
                else
                {
                    std::filesystem::create_directories(outputPath);
                }
                return;
            }

            if (std::filesystem::is_directory(outputPath))
            {
                return;
            }

            throw std::invalid_argument("Input path is a directory and given output path exists but is not a directory: " + outputPath.string());
        }

        if (std::filesystem::is_regular_file(inputPath))
        {
            if (!std::filesystem::exists(outputPath))
            {
                if (isFilePath(outputPath))
                {
                    auto const parent = outputPath.parent_path();
                    if (!parent.empty() && !std::filesystem::exists(parent))
                    {
                        std::filesystem::create_directories(parent);
                    }
                }
                else
                {
                    std::filesystem::create_directories(outputPath);
                }
                return;
            }

            if (std::filesystem::is_regular_file(outputPath) || std::filesystem::is_directory(outputPath))
            {
                return;
            }

            throw std::invalid_argument("Input path is a regular file and given output path exists but is not a directory or file: " + outputPath.string());
        }

        throw std::invalid_argument("Input path is not a directory and not a regular file: " + inputPath.string());
    }

    std::string normalizeExtension(std::filesystem::path const &path)
    {
        auto extension = path.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        return extension;
    }

    bool isFilePath(std::filesystem::path const &path)
    {
        if (!path.has_filename())
        {
            return false;
        }
        auto const name = path.filename().string();
        if (name == "." || name == "..")
        {
            return false;
        }
        if (!path.has_extension())
        {
            return false;
        }
        auto const extension = path.extension().string();
        if (extension == ".")
        {
            return false;
        }
        return true;
    }
}
