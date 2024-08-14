#include "../include/SinkManager.h"
#include "../include/InputElement.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{

    std::filesystem::path deriveOutputDirectoryPath(std::filesystem::path sourcePath, std::filesystem::path destinationPath)
    {
        sourcePath = sourcePath.lexically_normal();
        if (!std::filesystem::exists(sourcePath))
        {
            throw std::runtime_error(std::string("Source path does not exists: ") + sourcePath.string());
        }
        auto const canonicalSourceDirectoryPath = std::filesystem::canonical(std::filesystem::is_directory(sourcePath) ? sourcePath : sourcePath.parent_path());
        auto const relativeSourceDirectoryPath = std::filesystem::proximate(canonicalSourceDirectoryPath, std::filesystem::current_path());
        return (destinationPath / relativeSourceDirectoryPath).lexically_normal();
    }

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

        Internal(::utility::LoggerFactory &loggerFactory, std::filesystem::path sourcePath, std::filesystem::path dp)
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

    SinkManager::SinkManager(::utility::LoggerFactory &loggerFactory, std::filesystem::path sp, std::filesystem::path dp)
        : internal(std::make_shared<Internal>(loggerFactory, std::move(sp), std::move(dp)))
    {
    }

    std::filesystem::path SinkManager::deriveOutputElementPath(InputElement const &inputElement) const
    {
        if (internal->destinationIsFile)
        {
            return internal->destinationPath;
        }
        return (internal->destinationPath / inputElement.getUniquePath()).lexically_normal();
    }

    Writer SinkManager::get(InputElement const &inputElement) const
    {
        auto outputPath = deriveOutputElementPath(inputElement);
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
