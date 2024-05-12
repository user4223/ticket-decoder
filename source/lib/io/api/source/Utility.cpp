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

    void checkAndEnsureInputOutputPaths(std::filesystem::path const inputPath, std::optional<std::filesystem::path> optionalOutputPath)
    {

        if (std::filesystem::is_directory(inputPath))
        {
            if (optionalOutputPath)
            {
                if (!std::filesystem::exists(*optionalOutputPath))
                {
                    std::filesystem::create_directories(*optionalOutputPath);
                }
                else
                {
                    if (!std::filesystem::is_directory(*optionalOutputPath))
                    {
                        throw std::invalid_argument("Input path is a directory and output path is given, exists but is not a directory: " + optionalOutputPath->string());
                    }
                }
            }
        }
        else if (std::filesystem::is_regular_file(inputPath))
        {
            if (optionalOutputPath)
            {
                if (std::filesystem::exists(*optionalOutputPath) && !std::filesystem::is_regular_file(*optionalOutputPath))
                {
                    throw std::invalid_argument("Input path is a regular file and output path is given, exists but is not a regular file: " + optionalOutputPath->string());
                }
                std::filesystem::create_directories(optionalOutputPath->parent_path());
            }
        }
        else
        {
            throw std::invalid_argument("Input path is not a directory and not a regular file: " + inputPath.string());
        }
    }
}
