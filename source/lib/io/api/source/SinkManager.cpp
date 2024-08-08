#include "../include/SinkManager.h"
#include "../include/InputElement.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    std::filesystem::path deriveSourceDirectoryPath(std::filesystem::path sourcePath)
    {
        sourcePath = sourcePath.lexically_normal();
        if (!std::filesystem::exists(sourcePath))
        {
            throw std::runtime_error(std::string("Source path does not exists: ") + sourcePath.string());
        }
        return std::filesystem::canonical(std::filesystem::is_directory(sourcePath) ? sourcePath : sourcePath.parent_path());
    }

    std::filesystem::path deriveOutputDirectoryPath(std::filesystem::path sourcePath, std::filesystem::path destinationPath)
    {
        auto const relativePart = std::filesystem::proximate(deriveSourceDirectoryPath(sourcePath), std::filesystem::current_path());
        return (destinationPath / relativePart).lexically_normal();
    }

    struct SinkManager::Internal
    {
        ::utility::Logger logger;
        std::filesystem::path const outputDirectoryPath;

        Internal(::utility::LoggerFactory &loggerFactory, std::filesystem::path sourcePath, std::filesystem::path destinationPath)
            : logger(CREATE_LOGGER(loggerFactory)),
              outputDirectoryPath(std::move(destinationPath))
        {
            if (outputDirectoryPath.empty())
            {
                throw std::runtime_error("Expecting non-empty destination path to avoid overwrite of source elements");
            }
            LOG_DEBUG(logger) << "Output directory path: " << outputDirectoryPath;
        }
    };

    SinkManager::SinkManager(::utility::LoggerFactory &loggerFactory, std::filesystem::path sp, std::filesystem::path dp)
        : internal(std::make_shared<Internal>(loggerFactory, std::move(sp), std::move(dp)))
    {
    }

    std::filesystem::path SinkManager::deriveOutputElementPath(std::filesystem::path originalPath) const
    {
        return (internal->outputDirectoryPath / originalPath).lexically_normal();
    }

    Writer SinkManager::get(InputElement const &inputElement) const
    {
        auto outputPath = deriveOutputElementPath(inputElement.getUniquePath());
        LOG_INFO(internal->logger) << "Output item path: " << outputPath;
        return Writer(std::move(outputPath));
    }

    SinkManagerBuilder::SinkManagerBuilder(::utility::LoggerFactory &lf)
        : loggerFactory(lf)
    {
    }

    SinkManagerBuilder &SinkManagerBuilder::useSource(std::filesystem::path sp)
    {
        sourcePath = std::make_optional(sp);
        return *this;
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestination(std::filesystem::path dp)
    {
        destinationPath = dp;
        return *this;
    }

    SinkManager SinkManagerBuilder::build()
    {
        return SinkManager(loggerFactory, sourcePath.value_or(std::filesystem::current_path()), destinationPath);
    }

    SinkManagerBuilder SinkManager::create(::utility::LoggerFactory &loggerFactory)
    {
        return SinkManagerBuilder(loggerFactory);
    }
}
