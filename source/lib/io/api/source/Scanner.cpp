#include "../include/Scanner.h"

#include <regex>
#include <ostream>

namespace io::api
{
    std::vector<std::filesystem::path> Scanner::scan(std::filesystem::path directory, std::vector<std::string> extensions)
    {
        if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
        {
            if (std::filesystem::is_regular_file(directory))
            {
                return {directory};
            }
            return {};
        }

        auto result = std::vector<std::filesystem::path>{};
        auto joinedExtensions = std::stringstream();
        std::copy(extensions.begin(), extensions.end(), std::ostream_iterator<std::string>(joinedExtensions, "|"));
        auto const extensionRegex = std::regex("[.](" + joinedExtensions.str() + ")", std::regex_constants::icase);
        auto const hiddenRegex = std::regex("[.].*", std::regex_constants::icase);
        for (auto const &entry : std::filesystem::recursive_directory_iterator(directory))
        {
            auto const extension = entry.path().extension().string();
            auto const basename = entry.path().filename().string();
            if (std::regex_match(extension, extensionRegex) && !std::regex_match(basename, hiddenRegex))
            {
                result.push_back(entry.path());
            }
        }
        return result;
    }
}
