#pragma once

#include "Writer.h"

#include "lib/utility/include/LoggingFwd.h"

#include <filesystem>
#include <string>
#include <memory>
#include <optional>

namespace io::api
{
    class SinkManagerBuilder;
    class InputElement;

    class SinkManager
    {
        struct Internal;
        std::shared_ptr<Internal> internal;

    public:
        static bool isFilePath(std::filesystem::path const &path);

        SinkManager(::utility::LoggerFactory &loggerFactory);
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
        bool destinationStdout = false;

    public:
        friend SinkManager;

        SinkManagerBuilder(::utility::LoggerFactory &loggerFactory);

        SinkManagerBuilder &useDestination(std::filesystem::path destination);

        SinkManagerBuilder &useStdout();

        SinkManager build();
    };
}
