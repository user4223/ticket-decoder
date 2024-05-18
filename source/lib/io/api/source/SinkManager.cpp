#include "../include/SinkManager.h"
#include "../include/InputElement.h"

namespace io::api
{
    std::filesystem::path SinkManager::deriveSinkPath(std::filesystem::path originalPath, std::string extension) const
    {
        auto relative = sourcePath.extension().empty()
                            ? std::filesystem::proximate(originalPath, sourcePath)
                            : originalPath;
        auto finalDestination = (destinationPath / relative).lexically_normal();
        return finalDestination += extension;
    }

    Writer SinkManager::get(InputElement const &inputElement) const
    {
        return get(inputElement.getUniquePath().value()); // throws in case of camera input!
    }

    Writer SinkManager::get(std::filesystem::path originalPath) const
    {
        return Writer(deriveSinkPath(originalPath));
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
