
#include "../include/PathWriter.h"

#include "lib/utility/include/FileSystem.h"

#include <opencv2/imgcodecs.hpp>

#include <fstream>

namespace output::detail
{
    PathWriter::PathWriter(std::filesystem::path d, bool spe)
        : destination(std::move(d)),
          suppressPathExpansion(spe)
    {
        auto const parent = destination.parent_path();
        if (!parent.empty() && !std::filesystem::exists(parent))
        {
            if (!std::filesystem::create_directories(parent))
            {
                throw std::runtime_error(std::string("Creation of sink folder path failed: ") + parent.string());
            }
        }
    }

    std::filesystem::path PathWriter::deriveOutputPath(std::string postfix, std::string extension)
    {
        if (suppressPathExpansion)
        {
            auto const destinationExtension = utility::normalizeExtension(destination);
            if (destinationExtension == extension)
            {
                return destination;
            }
            auto base = destination;
            base += extension;
            return base;
        }
        auto base = destination;
        base += (postfix.empty() ? "" : "_" + postfix) + "_out" + extension;
        return base;
    }

    std::filesystem::path PathWriter::write(cv::Mat const &image, std::string postfix)
    {
        auto path = deriveOutputPath(postfix, ".png");
        cv::imwrite(path, image);
        return path;
    }

    std::filesystem::path PathWriter::write(std::vector<std::uint8_t> const &bytes, std::string postfix)
    {
        auto path = deriveOutputPath(postfix, ".raw");
        std::ofstream{path.string(), std::ios::binary}.write((char const *)&(bytes[0]), bytes.size());
        return path;
    }

    std::filesystem::path PathWriter::write(std::string const &json, std::string postfix)
    {
        auto path = deriveOutputPath(postfix, ".json");
        std::ofstream{path.string(), std::ios::binary}.write(json.data(), json.size());
        return path;
    }
}
