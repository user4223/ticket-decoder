#include "../include/SinkManager.h"

namespace io::api
{
    SinkManagerBuilder &SinkManagerBuilder::fromSource(std::filesystem::path sourcePath)
    {
        source = sourcePath;
        return *this;
    }

    SinkManagerBuilder &SinkManagerBuilder::deriveDestination(std::filesystem::path destinationPath)
    {
        destination = destinationPath;
        return *this;
    }

    SinkManager SinkManagerBuilder::build()
    {
        return SinkManager();
    }

    SinkManagerBuilder SinkManager::create()
    {
        return SinkManagerBuilder();
    }
}
