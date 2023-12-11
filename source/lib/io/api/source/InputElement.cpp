#include "../include/InputElement.h"

#include <numeric>

namespace io::api
{
    InputElement::InputElement(std::string a, cv::Mat &&i)
        : annotation(a), image(std::move(i))
    {
    }

    cv::Mat InputElement::getImage()
    {
        return image;
    }

    std::string InputElement::getAnnotation()
    {
        return annotation;
    }
}
