#pragma once

#include <opencv2/core.hpp>

#include <vector>
#include <string>

namespace io::api
{
    class InputElement
    {
        std::string annotation;
        cv::Mat image;

        InputElement(std::string annotation, cv::Mat &&image);

    public:
        static InputElement fromFile(std::string annotation, cv::Mat &&image);
        static InputElement fromCamera(cv::Mat &&image);

        InputElement(InputElement const &) = default;
        InputElement(InputElement &&) = default;
        InputElement &operator=(InputElement const &) = default;
        InputElement &operator=(InputElement &&) = default;

        bool isValid() const;

        cv::Mat getImage();

        std::string getAnnotation();
    };
}
