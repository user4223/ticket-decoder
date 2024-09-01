#include "../include/SinkManager.h"
#include "../include/InputElement.h"

#include "lib/utility/include/Logging.h"

namespace io::api
{
    struct Wrapper
    {
        virtual ~Wrapper() = default;
        virtual Writer get(InputElement const &inputElement) const = 0;
    };

    struct StreamWrapper : public Wrapper
    {
        std::ostream &stream;

        StreamWrapper(std::ostream &s) : stream(s) {}

        Writer get(InputElement const &inputElement) const override
        {
        }
    };

    struct PathWrapper
    {
        std::filesystem::path const destinationPath;
        bool const destinationIsFile;

        PathWrapper(std::filesystem::path dp)
            : destinationPath(std::move(dp)),
              destinationIsFile(isFilePath(destinationPath))
        {
            if (destinationPath.empty())
            {
                throw std::runtime_error("Expecting non-empty destination path to avoid overwrite of source elements");
            }
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

    SinkManager::SinkManager(infrastructure::Context &context, std::shared_ptr<Wrapper> w)
        : logger(CREATE_LOGGER(context.getLoggerFactory())),
          pathWrapper(),
          wrapper(std::move(w))
    {
    }

    SinkManager::SinkManager(infrastructure::Context &context, std::filesystem::path dp)
        : logger(CREATE_LOGGER(context.getLoggerFactory())),
          pathWrapper(std::make_shared<PathWrapper>(std::move(dp))),
          wrapper()
    {
        LOG_DEBUG(logger) << "Destination path: " << pathWrapper->destinationPath;
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
        LOG_INFO(logger) << "Output item path: " << outputPath;
        return Writer(std::move(outputPath), pathWrapper->destinationIsFile);
    }

    SinkManagerBuilder::SinkManagerBuilder(infrastructure::Context &c)
        : context(c)
    {
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestinationPath(std::filesystem::path dp)
    {
        destinationPath = std::make_optional(std::move(dp));
        return *this;
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestinationStream(std::ostream &stream)
    {
        wrapper = std::make_shared<StreamWrapper>(stream);
        return *this;
    }

    SinkManager SinkManagerBuilder::build()
    {
        if (!wrapper && !destinationPath)
        {
            throw std::runtime_error("No stream and no destination path specified, expecting at least one of them");
        }
        if (wrapper && destinationPath)
        {
            throw std::runtime_error("Stream and destination path specified, expecting one of them but not both");
        }

        if (destinationPath)
        {
            return SinkManager(context, *destinationPath);
        }
        else if (wrapper)
        {
            return SinkManager(context, std::move(wrapper));
        }
        throw std::runtime_error("Unimplmented sink type");
    }

    SinkManagerBuilder SinkManager::create(infrastructure::Context &context)
    {
        return SinkManagerBuilder(context);
    }
}
