#pragma once

#include "Writer.h"

#include "lib/infrastructure/include/Context.h"

#include <filesystem>
#include <memory>
#include <ostream>
#include <functional>
#include <optional>

namespace io::api
{
    class SinkManagerBuilder;
    class InputElement;
    struct SinkStrategy;

    class SinkManager
    {
        std::shared_ptr<SinkStrategy> wrapper;

    public:
        SinkManager(std::shared_ptr<SinkStrategy> wrapper);
        SinkManager(SinkManager const &) = delete;
        SinkManager(SinkManager &&) = default;
        SinkManager &operator=(SinkManager const &) = delete;
        SinkManager &operator=(SinkManager &&) = default;

        std::unique_ptr<Writer> get(std::optional<int> index = std::nullopt) const;
        std::unique_ptr<Writer> get(std::filesystem::path path, std::optional<int> index = std::nullopt) const;
        std::unique_ptr<Writer> get(InputElement const &inputElement) const;

        friend SinkManagerBuilder;

        static SinkManagerBuilder create(infrastructure::Context &context);
    };

    class SinkManagerBuilder
    {
        infrastructure::Context &context;
        std::shared_ptr<SinkStrategy> wrapper;

        SinkManagerBuilder(infrastructure::Context &context);

    public:
        friend SinkManager;

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
