#pragma once

#include "Writer.h"

#include "lib/utility/include/LoggingFwd.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <ostream>

namespace io::api
{
    class SinkManagerBuilder;
    class InputElement;
    struct StreamWrapper;
    struct PathWrapper;

    bool isFilePath(std::filesystem::path const &path);

    class SinkManager
    {
        ::utility::Logger logger;
        std::shared_ptr<PathWrapper> pathWrapper;
        std::shared_ptr<StreamWrapper> streamWrapper;

    public:
        SinkManager(::utility::LoggerFactory &loggerFactory, std::shared_ptr<StreamWrapper> stream);
        SinkManager(::utility::LoggerFactory &loggerFactory, std::filesystem::path destination);
        SinkManager(SinkManager const &) = delete;
        SinkManager(SinkManager &&) = default;
        SinkManager &operator=(SinkManager const &) = delete;
        SinkManager &operator=(SinkManager &&) = default;

        std::filesystem::path deriveOutputElementPath(InputElement const &inputElement) const;

        Writer get(InputElement const &inputElement) const;

        friend SinkManagerBuilder;

        static SinkManagerBuilder create(::utility::LoggerFactory &loggerFactory);
    };

    class SinkManagerBuilder
    {
        ::utility::LoggerFactory &loggerFactory;
        std::optional<std::filesystem::path> destinationPath = std::nullopt;
        std::shared_ptr<StreamWrapper> destinationStream;

    public:
        friend SinkManager;

        SinkManagerBuilder(::utility::LoggerFactory &loggerFactory);

        SinkManagerBuilder &useDestinationPath(std::filesystem::path destination);

        SinkManagerBuilder &useDestinationStream(std::ostream &stream);

        SinkManager build();
    };
}
