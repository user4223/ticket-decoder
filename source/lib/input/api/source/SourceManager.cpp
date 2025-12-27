// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/SourceManager.h"

#include "lib/input/camera/include/Camera.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/infrastructure/include/Logging.h"
#include "lib/utility/include/Utility.h"

namespace input::api
{
    static InputElement const empty = InputElement::empty();

    SourceManager::SourceManager(infrastructure::Context &c, LoadResult r, std::function<void(bool)> ctl)
        : logger(CREATE_LOGGER(c.getLoggerFactory())),
          loadResult(std::move(r)),
          cameraToggleListener(std::move(ctl)),
          currentElement(std::nullopt),
          selectedFileIndex(0),
          cameraEnabled(false),
          cameraPaused(false)
    {
        refresh();
    }

    SourceManager SourceManager::create(infrastructure::Context &context, LoadResult loadResult)
    {
        return create(context, std::move(loadResult), [](auto ignore) {});
    }

    SourceManager SourceManager::create(infrastructure::Context &context, LoadResult loadResult, std::function<void(bool)> cameraToggleListener)
    {
        return SourceManager(context, std::move(loadResult), std::move(cameraToggleListener));
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
        return cameraEnabled && !cameraPaused;
    }

    void SourceManager::refresh()
    {
        if (!cameraEnabled)
        {
            currentElement = selectedFileIndex < loadResult.size()
                                 ? std::make_optional(loadResult.get(selectedFileIndex))
                                 : std::nullopt;
        }
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
        if (cameraEnabled && cameraPaused)
        {
            cameraPaused = false;
            return std::to_string(cameraEnabled);
        }
        cameraEnabled = !cameraEnabled;
        cameraPaused = false;
        cameraToggleListener(cameraEnabled);
        if (!cameraEnabled)
        {
            camera::releaseCamera();
        }
        return std::to_string(cameraEnabled);
    }

    std::string SourceManager::pauseCamera()
    {
        if (!cameraEnabled)
        {
            return "0";
        }
        cameraPaused = true;
        return std::to_string(cameraPaused);
    }

    std::optional<InputElement> SourceManager::get()
    {
        if (cameraEnabled && !cameraPaused)
        {
            currentElement = std::make_optional(InputElement::fromCamera(camera::readCamera()));
        }
        return currentElement;
    }

    InputElement SourceManager::getOrWait()
    {
        auto element = get();
        if (cameraEnabled && element.has_value())
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
