#pragma once

#include "Reader.h"

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>
#include <map>

namespace io::api
{
    struct InputElement
    {
        std::string const annotation;
        cv::Mat const image;

        static std::vector<InputElement> create(Reader const &reader, std::filesystem::path path);
    };

    class Loader
    {
        std::map<std::string, std::shared_ptr<Reader>> const readers;

    public:
        Loader(std::vector<std::shared_ptr<Reader>> readers);

        std::vector<InputElement> load(std::filesystem::path path) const;

        static std::vector<std::filesystem::path> scan(std::filesystem::path directory, std::vector<std::string> extensions);
    };
}
