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
        std::filesystem::path deriveSinkPath(std::filesystem::path originalPath, std::string extension = std::string()) const;

        Writer get(InputElement const &inputElement) const;
        Writer get(std::filesystem::path originalPath) const;

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
