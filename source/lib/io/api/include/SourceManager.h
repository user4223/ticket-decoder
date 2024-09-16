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
        std::optional<InputElement> currentElement;
        unsigned int selectedFileIndex;
        bool cameraEnabled;

        SourceManager(infrastructure::Context &context, LoadResult loadResult);

        std::string getIdent() const;

    public:
        static SourceManager create(infrastructure::Context &context, LoadResult loadResult);

        bool isCameraEnabled() const;

        void refresh();

        std::string next();

        std::string previous();

        std::string toggleCamera();

        std::optional<InputElement> get() const;

        InputElement getOrWait();

        ParameterTypeList supplyParameters() const;
    };
}
