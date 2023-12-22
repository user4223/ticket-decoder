#include "../include/SinkManager.h"

namespace io::api
{

    std::filesystem::path SinkManager::deriveSinkPath(std::filesystem::path originalPath)
    {
        auto finalDestination = destinationPath / std::filesystem::relative(originalPath, sourcePath);
        return finalDestination.replace_extension();
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
        return SinkManagerBuilder();
    }
}
