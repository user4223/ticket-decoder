#include "../include/Reader.h"

#include <algorithm>

namespace io::api
{
    void Reader::validate(std::filesystem::path path, std::vector<std::string> allowedLowerCaseExtensions)
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error("Requested input file does not exist: " + path.string());
        }
        if (!std::filesystem::is_regular_file(path))
        {
            throw std::runtime_error("Requested input file is not a regular file: " + path.string());
        }
        auto extension = path.extension().string();
        if (!extension.empty() && extension[0] == '.')
        {
            extension = extension.substr(1);
        }
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        if (std::find(allowedLowerCaseExtensions.cbegin(), allowedLowerCaseExtensions.cend(), extension) == allowedLowerCaseExtensions.cend())
        {
            throw std::runtime_error("Requested input file has unexpected extension: " + path.string());
        }
    }
}
