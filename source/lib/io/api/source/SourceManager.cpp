#include "../include/SourceManager.h"

#include "lib/io/camera/include/Camera.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/utility/include/Logging.h"
#include "lib/utility/include/Utility.h"

namespace io::api
{
    static InputElement const empty = InputElement::empty();

    SourceManager::SourceManager(infrastructure::Context &context, LoadResult result)
        : logger(CREATE_LOGGER(context.getLoggerFactory())),
          loadResult(std::move(result)),
          currentElement(std::nullopt),
          selectedFileIndex(0),
          cameraEnabled(false)
    {
        refresh();
    }

    SourceManager SourceManager::create(infrastructure::Context &context, LoadResult loadResult)
    {
        return SourceManager(context, std::move(loadResult));
    }

    std::string SourceManager::getIdent() const
    {
        if (cameraEnabled)
        {
            return InputElement::CAMERA_ANNOTATION;
        }
        if (currentElement)
        {
            return currentElement->getAnnotation();
        }
        return InputElement::EMPTY_ANNOTATION;
    }

    bool SourceManager::isCameraEnabled() const
    {
        return cameraEnabled;
    }

    void SourceManager::refresh()
    {
        currentElement = selectedFileIndex < loadResult.size()
                             ? std::make_optional(loadResult.get(selectedFileIndex))
                             : std::nullopt;
    }

    std::string SourceManager::next()
    {
        ::utility::safeIncrement(selectedFileIndex, loadResult.size() - 1);
        refresh();
        return getIdent();
    }

    std::string SourceManager::previous()
    {
        ::utility::safeDecrement(selectedFileIndex, 0);
        refresh();
        return getIdent();
    }

    std::string SourceManager::toggleCamera()
    {
        cameraEnabled = !cameraEnabled;
        if (!cameraEnabled)
        {
            camera::releaseCamera();
        }
        return std::to_string(cameraEnabled);
    }

    std::optional<InputElement> SourceManager::get() const
    {
        return cameraEnabled
                   ? std::make_optional(InputElement::fromCamera(camera::readCamera()))
                   : currentElement;
    }

    InputElement SourceManager::getOrWait()
    {
        auto element = get();
        if (element.has_value() || cameraEnabled) // camera should always provide a value
        {
            return std::move(element.value());
        }

        if (!loadResult.waitForElementOrCompletion())
        {
            throw std::runtime_error("Load completed but no element found");
        }

        refresh();
        return get().value_or(InputElement::empty());
    }

    SourceManager::ParameterTypeList SourceManager::supplyParameters() const
    {
        return {std::make_pair("source:", getIdent())};
    }
}
