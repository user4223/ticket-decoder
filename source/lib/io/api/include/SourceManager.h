#pragma once

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/infrastructure/include/ParameterSupplier.h"

#include <lib/utility/include/Logger.h>

#include <lib/io/api/include/LoadResult.h>

#include <string>
#include <optional>

namespace io::api
{
    class SourceManager : public infrastructure::ParameterSupplier
    {
        ::utility::Logger logger;
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
