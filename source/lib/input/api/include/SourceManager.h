// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "../include/LoadResult.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/infrastructure/include/ParameterSupplier.h"
#include "lib/infrastructure/include/Logger.h"

#include <string>
#include <optional>

namespace input::api
{
    class SourceManager : public infrastructure::ParameterSupplier
    {
        infrastructure::Logger logger;
        LoadResult loadResult;
        std::function<void(bool)> cameraToggleListener;
        std::optional<InputElement> currentElement;
        unsigned int selectedFileIndex;
        bool cameraEnabled, cameraPaused;

        SourceManager(infrastructure::Context &context, LoadResult loadResult, std::function<void(bool)> cameraToggleListener);

        std::string getIdent() const;

    public:
        static SourceManager create(infrastructure::Context &context, LoadResult loadResult);
        static SourceManager create(infrastructure::Context &context, LoadResult loadResult, std::function<void(bool)> cameraToggleListener);

        bool isCameraEnabled() const;

        void refresh();

        std::string next();

        std::string previous();

        std::string toggleCamera();

        std::string pauseCamera();

        std::optional<InputElement> get();

        InputElement getOrWait();

        ParameterTypeList supplyParameters() const;
    };
}
