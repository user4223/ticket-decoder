#pragma once

#include "InputElement.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace io::api
{
    class SinkManagerBuilder;
    class SinkManager;

    class Sink
    {
        std::filesystem::path destination;

        Sink(std::filesystem::path destination);

    public:
        friend SinkManager;

        std::filesystem::path write(cv::Mat const &image);

        std::filesystem::path write(std::vector<uint8_t> const &bytes);

        std::filesystem::path write(std::string const &json);
    };

    class SinkManager
    {
        std::filesystem::path sourcePath;
        std::filesystem::path destinationPath;

    public:
        std::filesystem::path deriveSinkPath(std::filesystem::path originalPath, std::string extension = std::string()) const;

        Sink get(InputElement const &inputElement) const;
        Sink get(std::filesystem::path originalPath) const;

        friend SinkManagerBuilder;

        static SinkManagerBuilder create();
    };

    class SinkManagerBuilder
    {
        SinkManager sinkManager;

    public:
        friend SinkManager;

        SinkManagerBuilder &useSource(std::filesystem::path source);

        SinkManagerBuilder &useDestination(std::filesystem::path destination);

        SinkManager build();
    };
}
