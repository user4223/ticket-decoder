#include "../include/SourceManager.h"

#include "lib/io/camera/include/Camera.h"

#include "lib/utility/include/Logging.h"
#include "lib/utility/include/Utility.h"

namespace io::api
{
    static InputElement const empty = InputElement::empty();

    SourceManager::SourceManager(::utility::LoggerFactory &loggerFactory, LoadResult result)
        : logger(CREATE_LOGGER(loggerFactory)),
          loadResult(std::move(result)),
          currentElement(std::nullopt),
          selectedFileIndex(0),
          cameraEnabled(false)
    {
        refresh();
    }

    SourceManager SourceManager::create(::utility::LoggerFactory &loggerFactory, ::io::api::LoadResult loadResult)
    {
        return SourceManager(loggerFactory, std::move(loadResult));
    }

    std::string SourceManager::getIdent()
    {
        return cameraEnabled
                   ? "Camera"
                   : currentElement.value_or(empty).getAnnotation();
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
                   ? std::make_optional(camera::readCamera())
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
        return get().value();
    }
}
