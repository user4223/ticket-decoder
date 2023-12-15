#include "../include/InputElement.h"

#include <numeric>

namespace io::api
{
    InputElement::InputElement(std::string a, cv::Mat &&i)
        : annotation(a), image(std::move(i))
    {
    }

    InputElement InputElement::empty()
    {
        return InputElement("empty", cv::Mat{});
    }

    InputElement InputElement::fromFile(std::string annotation, cv::Mat &&image)
    {
        return InputElement(annotation, std::move(image));
    }
    InputElement InputElement::fromCamera(cv::Mat &&image)
    {
        return InputElement("camera", std::move(image));
    }

    bool InputElement::isValid() const { return !image.empty(); }

    cv::Mat InputElement::getImage()
    {
        return image;
    }

    std::string InputElement::getAnnotation()
    {
        return annotation;
    }
}
