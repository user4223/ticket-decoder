#pragma once

#include <filesystem>

namespace io::api
{
    class SinkManagerBuilder;

    class SinkManager
    {
        std::filesystem::path destinationPath;

    public:
        friend SinkManagerBuilder;

        static SinkManagerBuilder create();
    };

    class SinkManagerBuilder
    {
        std::filesystem::path source;
        std::filesystem::path destination;

    public:
        SinkManagerBuilder &fromSource(std::filesystem::path source);

        SinkManagerBuilder &deriveDestination(std::filesystem::path destination);

        SinkManager build();
    };
}
