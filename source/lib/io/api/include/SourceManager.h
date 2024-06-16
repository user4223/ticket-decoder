#pragma once

#include <lib/utility/include/LoggingFwd.h>

#include <lib/io/api/include/Loader.h>

#include <opencv2/core.hpp>

#include <string>
#include <optional>

namespace io::api
{
    class SourceManager
    {
        ::utility::Logger logger;
        LoadResult loadResult;
        std::optional<InputElement> currentElement;
        unsigned int selectedFileIndex;
        bool cameraEnabled;

        SourceManager(::utility::LoggerFactory &loggerFactory, LoadResult loadResult);

        std::string getIdent();

    public:
        static SourceManager create(::utility::LoggerFactory &loggerFactory, LoadResult loadResult);

        bool isCameraEnabled() const;

        void refresh();

        std::string next();

        std::string previous();

        std::string toggleCamera();

        std::optional<InputElement> get() const;

        InputElement getOrWait();

        template <typename IteratorT>
        void toString(IteratorT inserter)
        {
            *(inserter++) = std::make_pair("source:", getIdent());
        }
    };
}
