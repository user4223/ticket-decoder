#include "../include/InputElement.h"

#include <numeric>

namespace io::api
{
    std::string const InputElement::EMPTY_ANNOTATION = "empty";
    std::string const InputElement::CAMERA_ANNOTATION = "camera";
    InputElement const InputElement::emptyInputElement(InputElement::EMPTY_ANNOTATION, cv::Mat{});

    InputElement::InputElement(std::string a, cv::Mat &&i, std::optional<std::filesystem::path> p, std::optional<int> ix)
        : annotation(a), image(std::move(i)), path(p), index(ix)
    {
    }

    InputElement InputElement::empty()
    {
        return emptyInputElement;
    }

    InputElement InputElement::fromFile(std::filesystem::path path, cv::Mat &&image)
    {
        return InputElement(path.string(), std::move(image), std::make_optional(path));
    }

    InputElement InputElement::fromFile(std::filesystem::path path, int index, cv::Mat &&image)
    {
        return InputElement(path.string() + "[" + std::to_string(index) + "]", std::move(image), std::make_optional(path), std::make_optional(index));
    }

    InputElement InputElement::fromCamera(cv::Mat &&image)
    {
        return InputElement(CAMERA_ANNOTATION, std::move(image));
    }

    bool InputElement::isValid() const
    {
        return !image.empty();
    }

    bool InputElement::isVirtual() const
    {
        return !path.has_value();
    }

    cv::Mat InputElement::getImage() const
    {
        return image;
    }

    std::string InputElement::getAnnotation() const
    {
        return annotation;
    }

    std::optional<std::filesystem::path> InputElement::getPath() const
    {
        return path;
    }

    std::filesystem::path InputElement::getUniquePath() const
    {
        auto clone = path ? *path : std::filesystem::path(annotation);
        if (index.has_value())
        {
            clone += "_" + std::to_string(*index);
        }
        return clone;
    }
}
