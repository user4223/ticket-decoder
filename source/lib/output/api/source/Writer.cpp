
#include "../include/Writer.h"

#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/Base64.h"

#include <opencv2/imgcodecs.hpp>

#include <fstream>

namespace io::api
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

    struct StreamWriter::Internal
    {
        std::ostream &stream;
        std::string annotation;

        Internal(std::ostream &s, std::string a) : stream(s), annotation(std::move(a)) {}
    };

    StreamWriter::StreamWriter(std::ostream &stream, std::string annotation)
        : internal(std::make_shared<Internal>(stream, std::move(annotation)))
    {
    }

    std::filesystem::path StreamWriter::write(cv::Mat const &image, std::string postfix)
    {
        throw std::runtime_error("Cannot dump image to ostream");
    }

    std::filesystem::path StreamWriter::write(std::vector<std::uint8_t> const &bytes, std::string postfix)
    {
        return write(::utility::base64::encode(bytes), postfix);
    }

    std::filesystem::path StreamWriter::write(std::string const &json, std::string postfix)
    {
        internal->stream << json << std::endl;
        return internal->annotation;
    }
}
