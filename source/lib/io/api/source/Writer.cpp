#include "../include/Writer.h"
#include "../include/Utility.h"

#include <opencv2/imgcodecs.hpp>

#include <fstream>

namespace io::api
{

    Writer::Writer(std::filesystem::path d, bool spe)
        : destination(std::move(d)),
          suppressPathExpansion(spe)
    {
        if (!std::filesystem::exists(destination.parent_path()))
        {
            if (!std::filesystem::create_directories(destination.parent_path()))
            {
                throw std::runtime_error(std::string("Creation of sink folder path failed: ") + destination.parent_path().string());
            }
        }
    }

    std::filesystem::path Writer::deriveOutputPath(std::string postfix, std::string extension)
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

    std::filesystem::path Writer::write(cv::Mat const &image, std::string postfix)
    {
        auto path = deriveOutputPath(postfix, ".png");
        cv::imwrite(path, image);
        return path;
    }

    std::filesystem::path Writer::write(std::vector<std::uint8_t> const &bytes, std::string postfix)
    {
        auto path = deriveOutputPath(postfix, ".raw");
        std::ofstream{path.string(), std::ios::binary}.write((char const *)&(bytes[0]), bytes.size());
        return path;
    }

    std::filesystem::path Writer::write(std::string const &json, std::string postfix)
    {
        auto path = deriveOutputPath(postfix, ".json");
        std::ofstream{path.string(), std::ios::binary}.write(json.data(), json.size());
        return path;
    }
}
