#include "../include/SinkManager.h"
#include "../include/InputElement.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    struct SinkManager::Internal
    {
        ::utility::Logger logger;
        std::filesystem::path const sourcePath;
        std::filesystem::path const destinationPath;

        Internal(::utility::LoggerFactory &loggerFactory, std::filesystem::path sp, std::filesystem::path dp)
            : logger(CREATE_LOGGER(loggerFactory)), sourcePath(std::move(sp)), destinationPath(std::move(dp))
        {
        }
    };

    SinkManager::SinkManager(::utility::LoggerFactory &loggerFactory, std::filesystem::path sp, std::filesystem::path dp)
        : internal(std::make_shared<Internal>(loggerFactory, std::move(sp), std::move(dp)))
    {
    }

    std::filesystem::path SinkManager::deriveSinkPath(std::filesystem::path originalPath, std::string extension) const
    {
        auto relative = internal->sourcePath.extension().empty()
                            ? std::filesystem::proximate(originalPath, internal->sourcePath)
                            : originalPath;
        auto finalDestination = (internal->destinationPath / relative).lexically_normal();
        finalDestination += extension;

        LOG_DEBUG(internal->logger) << "Sink path: " << finalDestination;
        return finalDestination;
    }

    Writer SinkManager::get(InputElement const &inputElement) const
    {
        return get(inputElement.getUniquePath().value()); // throws in case of camera input!
    }

    Writer SinkManager::get(std::filesystem::path originalPath) const
    {
        return Writer(deriveSinkPath(originalPath));
    }

    SinkManagerBuilder::SinkManagerBuilder(::utility::LoggerFactory &lf)
        : loggerFactory(lf)
    {
    }

    SinkManagerBuilder &SinkManagerBuilder::useSource(std::filesystem::path sp)
    {
        sourcePath = sp;
        return *this;
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestination(std::filesystem::path dp)
    {
        destinationPath = dp;
        return *this;
    }

    SinkManager SinkManagerBuilder::build()
    {
        return SinkManager(loggerFactory, sourcePath, destinationPath);
    }

    SinkManagerBuilder SinkManager::create(::utility::LoggerFactory &loggerFactory)
    {
        auto builder = SinkManagerBuilder(loggerFactory);
        builder.sourcePath = std::filesystem::current_path();
        return std::move(builder);
    }
}
