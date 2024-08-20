#include "../include/SinkManager.h"
#include "../include/InputElement.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    struct StreamWrapper
    {
        std::ostream &stream;

        StreamWrapper(std::ostream &s) : stream(s) {}
    };

    struct PathWrapper
    {
        ::utility::Logger logger;
        std::filesystem::path const destinationPath;
        bool const destinationIsFile;

        PathWrapper(::utility::LoggerFactory &loggerFactory, std::filesystem::path dp)
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

    bool isFilePath(std::filesystem::path const &path)
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

    SinkManager::SinkManager(::utility::LoggerFactory &loggerFactory, std::shared_ptr<StreamWrapper> s)
        : pathWrapper(),
          streamWrapper(std::move(s))
    {
    }

    SinkManager::SinkManager(::utility::LoggerFactory &loggerFactory, std::filesystem::path dp)
        : pathWrapper(std::make_shared<PathWrapper>(loggerFactory, std::move(dp))),
          streamWrapper()
    {
    }

    std::filesystem::path SinkManager::deriveOutputElementPath(InputElement const &inputElement) const
    {
        if (pathWrapper->destinationIsFile)
        {
            auto const index = inputElement.getIndex();
            if (!index || *index == 0)
            {
                return pathWrapper->destinationPath;
            }
            auto steam = pathWrapper->destinationPath.stem().string();
            auto parent = pathWrapper->destinationPath.parent_path();
            return parent / (steam + "_" + std::to_string(*index) + pathWrapper->destinationPath.extension().string());
        }
        return (pathWrapper->destinationPath / inputElement.getUniquePath()).lexically_normal();
    }

    Writer SinkManager::get(InputElement const &inputElement) const
    {
        auto outputPath = deriveOutputElementPath(inputElement);
        LOG_INFO(pathWrapper->logger) << "Output item path: " << outputPath;
        return Writer(std::move(outputPath), pathWrapper->destinationIsFile);
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
        destinationStream = std::make_shared<StreamWrapper>(stream);
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
