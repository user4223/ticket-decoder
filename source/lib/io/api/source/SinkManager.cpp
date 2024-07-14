#include "../include/SinkManager.h"
#include "../include/InputElement.h"

namespace io::api
{
    SinkManager::SinkManager(std::filesystem::path sp, std::filesystem::path dp)
        : sourcePath(std::move(sp)), destinationPath(std::move(dp))
    {
    }

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
        return SinkManager(sourcePath, destinationPath);
    }

    SinkManagerBuilder SinkManager::create()
    {
        SinkManagerBuilder builder;
        builder.sourcePath = std::filesystem::current_path();
        return std::move(builder);
    }
}
