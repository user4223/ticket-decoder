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

    public:
        InputElement(std::string annotation, cv::Mat &&image);
        InputElement(InputElement const &) = default;
        InputElement(InputElement &&) = default;
        InputElement &operator=(InputElement const &) = default;
        InputElement &operator=(InputElement &&) = default;

        cv::Mat getImage();

        std::string getAnnotation();
    };
}
