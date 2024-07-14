#pragma once

#include "Writer.h"

#include <filesystem>
#include <string>

namespace io::api
{
    class SinkManagerBuilder;
    class InputElement;

    class SinkManager
    {
        std::filesystem::path sourcePath;
        std::filesystem::path destinationPath;

    public:
        SinkManager(std::filesystem::path source, std::filesystem::path destination);
        SinkManager(SinkManager const &) = delete;
        SinkManager(SinkManager &&) = default;
        SinkManager &operator=(SinkManager const &) = delete;
        SinkManager &operator=(SinkManager &&) = default;

        std::filesystem::path deriveSinkPath(std::filesystem::path originalPath, std::string extension = std::string()) const;

        Writer get(InputElement const &inputElement) const;
        Writer get(std::filesystem::path originalPath) const;

        friend SinkManagerBuilder;

        static SinkManagerBuilder create();
    };

    class SinkManagerBuilder
    {
        std::filesystem::path sourcePath;
        std::filesystem::path destinationPath;

    public:
        friend SinkManager;

        SinkManagerBuilder &useSource(std::filesystem::path source);

        SinkManagerBuilder &useDestination(std::filesystem::path destination);

        SinkManager build();
    };
}
