#pragma once

#include <lib/utility/include/LoggingFwd.h>

#include <memory>
#include <string>
#include <filesystem>

namespace api
{
    class DecoderFacade
    {
        struct Internal;

        std::shared_ptr<Internal> internal;

    public:
        DecoderFacade(::utility::LoggerFactory &loggerFactory);

        std::string decodeUIC918(std::string base64RawData);

        std::vector<std::string> decodeFile(std::filesystem::path filePath);
    };
}
