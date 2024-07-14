#include "../include/Writer.h"

#include <opencv2/imgcodecs.hpp>

#include <fstream>

namespace io::api
{

    Writer::Writer(std::filesystem::path d)
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

    std::filesystem::path Writer::write(cv::Mat const &image, std::string postfix)
    {
        auto clone = destination;
        clone += (postfix.empty() ? "" : "_" + postfix) + "_out.png";
        cv::imwrite(clone, image);
        return clone;
    }

    std::filesystem::path Writer::write(std::vector<std::uint8_t> const &bytes, std::string postfix)
    {
        auto clone = destination;
        clone += (postfix.empty() ? "" : "_" + postfix) + "_out.raw";
        std::ofstream{clone.string(), std::ios::binary}.write((char const *)&(bytes[0]), bytes.size());
        return clone;
    }

    std::filesystem::path Writer::write(std::string const &json, std::string postfix)
    {
        auto clone = destination;
        clone += (postfix.empty() ? "" : "_" + postfix) + "_out.json";
        std::ofstream{clone.string(), std::ios::binary}.write(json.data(), json.size());
        return clone;
    }
}
