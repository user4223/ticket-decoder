#pragma once

#include "Writer.h"

#include "lib/infrastructure/include/Context.h"

#include <filesystem>
#include <memory>
#include <optional>
#include <ostream>
#include <functional>

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
        SinkManager(infrastructure::Context &context, std::shared_ptr<StreamWrapper> stream);
        SinkManager(infrastructure::Context &context, std::filesystem::path destination);
        SinkManager(SinkManager const &) = delete;
        SinkManager(SinkManager &&) = default;
        SinkManager &operator=(SinkManager const &) = delete;
        SinkManager &operator=(SinkManager &&) = default;

        std::filesystem::path deriveOutputElementPath(InputElement const &inputElement) const;

        Writer get(InputElement const &inputElement) const;

        friend SinkManagerBuilder;

        static SinkManagerBuilder create(infrastructure::Context &context);
    };

    class SinkManagerBuilder
    {
        infrastructure::Context &context;
        std::optional<std::filesystem::path> destinationPath = std::nullopt;
        std::shared_ptr<StreamWrapper> destinationStream;

    public:
        friend SinkManager;

        SinkManagerBuilder(infrastructure::Context &context);

        SinkManagerBuilder &useDestinationPath(std::filesystem::path destination);

        SinkManagerBuilder &useDestinationStream(std::ostream &stream);

        SinkManagerBuilder &use(std::function<void(SinkManagerBuilder &)> modifier)
        {
            modifier(*this);
            return *this;
        }

        SinkManager build();
    };
}
