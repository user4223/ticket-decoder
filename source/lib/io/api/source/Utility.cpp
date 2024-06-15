#include "../include/Utility.h"

#include <algorithm>
#include <iostream>
#include <fstream>

namespace io::api::utility
{
    struct OutputStream::Internal
    {
        std::ofstream fileStream;

        Internal(std::ofstream fs) : fileStream(std::move(fs)) {}
    };

    OutputStream::OutputStream(std::filesystem::path filePath)
        : internal(std::make_shared<Internal>(std::ofstream(filePath, std::ios::out | std::ios::trunc))),
          stream(internal->fileStream)
    {
    }

    OutputStream::OutputStream()
        : internal(),
          stream(std::cout)
    {
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
}
