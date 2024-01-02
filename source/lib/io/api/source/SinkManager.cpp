#include "../include/SinkManager.h"

namespace io::api
{

    std::filesystem::path SinkManager::deriveSinkPath(std::filesystem::path originalPath)
    {
        auto relative = sourcePath.extension().empty()
                            ? std::filesystem::relative(originalPath, sourcePath)
                            : originalPath;
        auto finalDestination = destinationPath / relative;
        return finalDestination.lexically_normal().replace_extension();
    }

    SinkManager &SinkManager::handleImage(std::filesystem::path originalPath)
    {
        return *this;
    }

    SinkManager &SinkManager::handleRaw(std::filesystem::path originalPath)
    {
        return *this;
    }

    SinkManager &SinkManager::handleJson(std::filesystem::path originalPath)
    {
        return *this;
    }

    SinkManagerBuilder &SinkManagerBuilder::useSource(std::filesystem::path sourcePath)
    {
        sinkManager.sourcePath = sourcePath;
        return *this;
    }

    SinkManagerBuilder &SinkManagerBuilder::useDestination(std::filesystem::path destinationPath)
    {
        sinkManager.destinationPath = destinationPath;
        return *this;
    }

    SinkManager SinkManagerBuilder::build()
    {
        return std::move(sinkManager);
    }

    SinkManagerBuilder SinkManager::create()
    {
        auto builder = SinkManagerBuilder();
        builder.sinkManager.sourcePath = std::filesystem::current_path();
        return builder;
    }
}
