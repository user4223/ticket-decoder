#include "../include/InputElement.h"

#include <numeric>

namespace io::api
{
    InputElement::InputElement(std::string a, cv::Mat &&i, std::optional<std::filesystem::path> p, std::optional<int> ix)
        : annotation(a), path(p), index(ix), image(std::move(i))
    {
    }

    InputElement InputElement::empty()
    {
        return InputElement("empty", cv::Mat{});
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
        return InputElement("camera", std::move(image));
    }

    bool InputElement::isValid() const
    {
        return !image.empty();
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

    std::optional<std::filesystem::path> InputElement::getUniqueStrippedPath() const
    {
        if (!path)
        {
            return std::nullopt;
        }
        auto const name = path->stem().string();
        auto const postfix = index.has_value() ? (std::string("_") + std::to_string(*index)) : std::string();
        return path->parent_path().append(name + postfix);
    }
}
