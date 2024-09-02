#include "../include/Utility.h"

#include <algorithm>
#include <iostream>
#include <fstream>

namespace io::api::utility
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

    void checkAndEnsureCompatiblePaths(std::filesystem::path const inputPath, std::filesystem::path const outputPath)
    {

        if (std::filesystem::is_directory(inputPath))
        {
            if (!std::filesystem::exists(outputPath))
            {
                std::filesystem::create_directories(outputPath);
            }
            else
            {
                if (!std::filesystem::is_directory(outputPath))
                {
                    throw std::invalid_argument("Input path is a directory and given output path exists but is not a directory: " + outputPath.string());
                }
            }
        }
        else if (std::filesystem::is_regular_file(inputPath))
        {
            if (std::filesystem::exists(outputPath) && !std::filesystem::is_regular_file(outputPath))
            {
                throw std::invalid_argument("Input path is a regular file and given output path exists but is not a regular file: " + outputPath.string());
            }
            std::filesystem::create_directories(outputPath.parent_path());
        }
        else
        {
            throw std::invalid_argument("Input path is not a directory and not a regular file: " + inputPath.string());
        }
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
