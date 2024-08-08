#pragma once

#include <opencv2/core.hpp>

#include <vector>
#include <string>
#include <filesystem>
#include <optional>

namespace io::api
{
    class InputElement
    {
        static InputElement const emptyInputElement;

        std::string annotation;
        cv::Mat image;
        std::optional<std::filesystem::path> path;
        std::filesystem::path relativeUniquePath;

        InputElement(
            std::string annotation,
            cv::Mat &&image,
            std::optional<std::filesystem::path> path = std::nullopt,
            std::optional<int> index = std::nullopt);

    public:
        static std::string const EMPTY_ANNOTATION;
        static std::string const CAMERA_ANNOTATION;

        static std::filesystem::path removeLeadingRelativeParts(std::filesystem::path const &in);
        static std::filesystem::path createRelativeUniquePath(std::filesystem::path const &path, std::optional<int> index);

        static InputElement empty();
        static InputElement fromFile(std::filesystem::path path, cv::Mat &&image);
        static InputElement fromFile(std::filesystem::path path, int index, cv::Mat &&image);
        static InputElement fromCamera(cv::Mat &&image);

        InputElement(InputElement const &) = default;
        InputElement(InputElement &&) = default;
        InputElement &operator=(InputElement const &) = default;
        InputElement &operator=(InputElement &&) = default;

        InputElement &replaceImage(cv::Mat &&image);

        bool isValid() const;

        bool isVirtual() const;

        cv::Mat getImage() const;

        std::string getAnnotation() const;

        std::optional<std::filesystem::path> getPath() const;

        std::filesystem::path getUniquePath() const;
    };
}
