
#include "../include/TempPath.h"

#include <algorithm>

namespace test::support
{
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string generateRandomString(size_t length)
    {
        auto randchar = []() -> char
        {
            const size_t max_index = (sizeof(charset) - 1);
            return charset[rand() % max_index];
        };
        std::string str(length, 0);
        std::generate_n(str.begin(), length, randchar);
        return str;
    }

    TempPath::TempPath(bool changeCurrentDirectory)
        : path(std::filesystem::temp_directory_path() / generateRandomString(20))
    {
        std::filesystem::create_directories(path);
        if (changeCurrentDirectory)
        {
            std::filesystem::current_path(path);
        }
    }

    TempPath::~TempPath()
    {
        std::error_code ec;
        std::filesystem::remove_all(path, ec);
    }
}
