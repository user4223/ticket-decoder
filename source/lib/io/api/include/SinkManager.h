#pragma once

#include <filesystem>

namespace io::api
{
    class SinkManagerBuilder;

    class SinkManager
    {
        std::filesystem::path sourcePath;
        std::filesystem::path destinationPath;

    public:
        std::filesystem::path deriveSinkPath(std::filesystem::path originalPath);

        SinkManager &handleImage(std::filesystem::path originalPath);

        SinkManager &handleRaw(std::filesystem::path originalPath);

        SinkManager &handleJson(std::filesystem::path originalPath);

        friend SinkManagerBuilder;

        static SinkManagerBuilder create();
    };

    class SinkManagerBuilder
    {
        SinkManager sinkManager;

    public:
        SinkManagerBuilder &useSource(std::filesystem::path source);

        SinkManagerBuilder &useDestination(std::filesystem::path destination);

        SinkManager build();
    };
}
