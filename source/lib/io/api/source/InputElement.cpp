#include "../include/InputElement.h"

#include <numeric>
#include <regex>

namespace io::api
{
    std::string const InputElement::EMPTY_ANNOTATION = "empty";
    std::string const InputElement::CAMERA_ANNOTATION = "camera";
    InputElement const InputElement::emptyInputElement(InputElement::EMPTY_ANNOTATION, cv::Mat{});

    static std::regex const parentDirectoryRegex = std::regex("^([.][.]?[/])+|^[.]$");

    std::filesystem::path InputElement::removeLeadingRelativeParts(std::filesystem::path path)
    {
        return std::filesystem::path(std::regex_replace(path.string(), parentDirectoryRegex, ""));
    }

    std::filesystem::path InputElement::appendOptionalIndex(std::filesystem::path path, std::optional<int> index)
    {
        return index.has_value() ? path.concat("_" + std::to_string(*index)) : path;
    }

    std::filesystem::path InputElement::createRelativeUniquePath(std::filesystem::path path, std::optional<int> index)
    {
        return appendOptionalIndex(removeLeadingRelativeParts(path.is_absolute() ? std::filesystem::relative(path) : path.lexically_normal()), index);
    }

    InputElement::InputElement(std::string a, cv::Mat &&i, std::optional<std::filesystem::path> p, std::optional<int> ix)
        : annotation(a),
          image(std::move(i)),
          path(p),
          relativeUniquePath(path ? createRelativeUniquePath(*path, ix) : std::filesystem::path(annotation))
    {
    }

    InputElement &InputElement::replaceImage(cv::Mat &&i)
    {
        image = std::move(i);
        return *this;
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
        return relativeUniquePath;
    }
}
