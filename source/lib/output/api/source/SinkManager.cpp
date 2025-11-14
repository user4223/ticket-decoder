
#include "../include/SinkManager.h"

#include "lib/input/api/include/InputElement.h"
#include "lib/input/common/include/Utility.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

namespace io::api
{
    struct SinkStrategy
    {
        virtual ~SinkStrategy() = default;
        virtual std::unique_ptr<Writer> get(std::optional<std::filesystem::path> path, std::optional<int> index) const = 0;
    };

    struct StreamSinkStrategy : public SinkStrategy
    {
        ::utility::Logger logger;
        std::ostream &stream;

        StreamSinkStrategy(::utility::LoggerFactory &loggerFactory, std::ostream &s)
            : logger(CREATE_LOGGER(loggerFactory)),
              stream(s)
        {
            LOG_DEBUG(logger) << "Destination is stream";
        }

        std::unique_ptr<Writer> get(std::optional<std::filesystem::path> path, std::optional<int> index) const override
        {
            return std::make_unique<StreamWriter>(stream, path.value_or(""));
        }
    };

    struct PathSinkStrategy : public SinkStrategy
    {
        ::utility::Logger logger;
        std::filesystem::path const destinationPath;
        bool const destinationIsFile;

        PathSinkStrategy(::utility::LoggerFactory &loggerFactory, std::filesystem::path dp)
            : logger(CREATE_LOGGER(loggerFactory)),
              destinationPath(std::move(dp)),
              destinationIsFile(utility::isFilePath(destinationPath))
        {
            if (destinationPath.empty())
            {
                throw std::runtime_error("Expecting non-empty destination path to avoid overwrite of source elements");
            }
            LOG_DEBUG(logger) << "Destination path: " << destinationPath;
        }

        std::filesystem::path deriveOutputElementPath(std::optional<std::filesystem::path> path, std::optional<int> index) const
        {
            if (destinationIsFile)
            {
                if (!index || *index == 0)
                {
                    return destinationPath;
                }
                auto steam = destinationPath.stem().string();
                auto parent = destinationPath.parent_path();
                return parent / (steam + "_" + std::to_string(*index) + destinationPath.extension().string());
            }

            if (!path)
            {
                throw std::runtime_error("Destination path is a directory but input element path is missing, unable to derive destination file path");
            }
            return (destinationPath / *path).lexically_normal();
        }

        std::unique_ptr<Writer> get(std::optional<std::filesystem::path> path, std::optional<int> index) const override
        {
            auto outputPath = deriveOutputElementPath(path, index);
            LOG_DEBUG(logger) << "Output item path: " << outputPath;
            return std::make_unique<PathWriter>(std::move(outputPath), destinationIsFile);
        }
    };

    SinkManager::SinkManager(std::shared_ptr<SinkStrategy> w)
        : wrapper(std::move(w))
    {
    }

    std::unique_ptr<Writer> SinkManager::get(std::optional<int> index) const
    {
        return wrapper->get(std::nullopt, index);
    }

    std::unique_ptr<Writer> SinkManager::get(InputElement const &inputElement) const
    {
        return wrapper->get(inputElement.getUniquePath(), inputElement.getIndex());
    }

    std::unique_ptr<Writer> SinkManager::get(std::filesystem::path path, std::optional<int> index) const
    {
        return wrapper->get(path, index);
    }

    SinkManagerBuilder::SinkManagerBuilder(infrastructure::Context &c)
        : context(c)
    {
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestinationPath(std::filesystem::path dp)
    {
        if (wrapper)
        {
            throw std::runtime_error("Destination has been specified already, cannot overwrite it with destination path: " + dp.string());
        }
        wrapper = std::make_shared<PathSinkStrategy>(context.getLoggerFactory(), std::move(dp));
        return *this;
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestinationStream(std::ostream &stream)
    {
        if (wrapper)
        {
            throw std::runtime_error("Destination has been specified already, cannot overwrite it with stream");
        }
        wrapper = std::make_shared<StreamSinkStrategy>(context.getLoggerFactory(), stream);
        return *this;
    }

    SinkManager SinkManagerBuilder::build()
    {
        if (!wrapper)
        {
            throw std::runtime_error("No stream and no destination path specified, expecting at least one of them");
        }
        return SinkManager(std::move(wrapper));
    }

    SinkManagerBuilder SinkManager::create(infrastructure::Context &context)
    {
        return SinkManagerBuilder(context);
    }
}
