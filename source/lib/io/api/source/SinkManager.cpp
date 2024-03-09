#include "../include/SinkManager.h"

#include <opencv2/imgcodecs.hpp>

#include <fstream>

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
        if (clone.extension() != ".png")
        {
            clone += ".png";
        }
        cv::imwrite(clone, image);
        return clone;
    }

    std::filesystem::path Sink::write(std::vector<uint8_t> const &bytes)
    {
        auto clone = destination;
        if (clone.extension() != ".raw")
        {
            clone += ".raw";
        }
        std::ofstream{clone.string(), std::ios::binary}.write((char const *)&(bytes[0]), bytes.size());
        return clone;
    }

    std::filesystem::path Sink::write(std::string const &json)
    {
        auto clone = destination;
        if (clone.extension() != ".json")
        {
            clone += ".json";
        }
        std::ofstream{clone.string(), std::ios::binary}.write(json.data(), json.size());
        return clone;
    }

    std::filesystem::path SinkManager::deriveSinkPath(std::filesystem::path originalPath, std::string extension) const
    {
        auto relative = sourcePath.extension().empty()
                            ? std::filesystem::relative(originalPath, sourcePath)
                            : originalPath;
        auto finalDestination = (destinationPath / relative).lexically_normal();
        finalDestination += extension;
        return finalDestination;
    }

    Sink SinkManager::get(InputElement const &inputElement) const
    {
        return get(inputElement.getUniquePath().value()); // throws in case of camera input!
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
