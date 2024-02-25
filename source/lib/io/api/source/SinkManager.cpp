#include "../include/SinkManager.h"

#include <opencv2/imgcodecs.hpp>

namespace io::api
{

    Sink::Sink(std::filesystem::path d)
        : destination(std::move(d))
    {
        if (!std::filesystem::exists(destination.parent_path()))
        {
            if (!std::filesystem::create_directories(destination.parent_path()))
            {
                throw std::runtime_error(std::string("Creation of sink folder path failed: ") + destination.parent_path().string());
            }
        }
    }

    std::filesystem::path Sink::write(cv::Mat const &image)
    {
        auto clone = destination;
        auto const imageFile = clone.replace_extension(".png");
        cv::imwrite(imageFile, image);
        return imageFile;
    }

    std::filesystem::path Sink::write(std::vector<uint8_t> const &bytes)
    {
        auto clone = destination;
        return clone.replace_extension(".raw");
    }

    std::filesystem::path Sink::write(std::string const &json)
    {
        auto clone = destination;
        return clone.replace_extension(".json");
    }

    std::filesystem::path SinkManager::deriveSinkPath(std::filesystem::path originalPath, std::string extension) const
    {
        auto relative = sourcePath.extension().empty()
                            ? std::filesystem::relative(originalPath, sourcePath)
                            : originalPath;
        auto finalDestination = destinationPath / relative;
        return finalDestination.lexically_normal().replace_extension(extension);
    }

    Sink SinkManager::get(InputElement const &inputElement) const
    {
        return get(inputElement.getUniqueStrippedPath().value()); // throws in case of camera input!
    }

    Sink SinkManager::get(std::filesystem::path originalPath) const
    {
        return Sink(deriveSinkPath(originalPath));
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
