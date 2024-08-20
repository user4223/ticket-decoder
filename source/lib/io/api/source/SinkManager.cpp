#include "../include/SinkManager.h"
#include "../include/InputElement.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    struct OstreamWrapper
    {
        std::ostream &stream;

        OstreamWrapper(std::ostream &s) : stream(s) {}
    };

    bool SinkManager::isFilePath(std::filesystem::path const &path)
    {
        if (!path.has_filename())
        {
            return false;
        }
        auto const name = path.filename().string();
        if (name == "." || name == "..")
        {
            return false;
        }
        if (!path.has_extension())
        {
            return false;
        }
        auto const extension = path.extension().string();
        if (extension == ".")
        {
            return false;
        }
        return true;
    }

    struct SinkManager::Internal
    {
        ::utility::Logger logger;
        std::filesystem::path const destinationPath;
        bool const destinationIsFile;

        Internal(::utility::LoggerFactory &loggerFactory, std::filesystem::path dp)
            : logger(CREATE_LOGGER(loggerFactory)),
              destinationPath(std::move(dp)),
              destinationIsFile(isFilePath(destinationPath))
        {
            if (destinationPath.empty())
            {
                throw std::runtime_error("Expecting non-empty destination path to avoid overwrite of source elements");
            }
            LOG_DEBUG(logger) << "Destination path: " << destinationPath;
        }
    };

    SinkManager::SinkManager(::utility::LoggerFactory &loggerFactory, std::shared_ptr<OstreamWrapper> s)
        : internal()
    {
    }

    SinkManager::SinkManager(::utility::LoggerFactory &loggerFactory, std::filesystem::path dp)
        : internal(std::make_shared<Internal>(loggerFactory, std::move(dp)))
    {
    }

    std::filesystem::path SinkManager::deriveOutputElementPath(InputElement const &inputElement) const
    {
        if (internal->destinationIsFile)
        {
            auto const index = inputElement.getIndex();
            if (!index || *index == 0)
            {
                return internal->destinationPath;
            }
            auto steam = internal->destinationPath.stem().string();
            auto path = internal->destinationPath.parent_path();
            return path / (steam + "_" + std::to_string(*index) + internal->destinationPath.extension().string());
        }
        return (internal->destinationPath / inputElement.getUniquePath()).lexically_normal();
    }

    Writer SinkManager::get(InputElement const &inputElement) const
    {
        auto outputPath = deriveOutputElementPath(inputElement);
        LOG_INFO(internal->logger) << "Output item path: " << outputPath;
        return Writer(std::move(outputPath), internal->destinationIsFile);
    }

    SinkManagerBuilder::SinkManagerBuilder(::utility::LoggerFactory &lf)
        : loggerFactory(lf)
    {
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestinationPath(std::filesystem::path dp)
    {
        destinationPath = std::make_optional(std::move(dp));
        return *this;
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestinationStream(std::ostream &stream)
    {
        destinationStream = std::make_shared<OstreamWrapper>(stream);
        return *this;
    }

    SinkManager SinkManagerBuilder::build()
    {
        if (!destinationStream && !destinationPath)
        {
            throw std::runtime_error("No stream and no destination path specified, expecting at least one of them");
        }
        if (destinationStream && destinationPath)
        {
            throw std::runtime_error("Stream and destination path specified, expecting one of them but not both");
        }
        return SinkManager(loggerFactory, *destinationPath);
    }

    SinkManagerBuilder SinkManager::create(::utility::LoggerFactory &loggerFactory)
    {
        return SinkManagerBuilder(loggerFactory);
    }
}
