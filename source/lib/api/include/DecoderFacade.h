#pragma once

#include <lib/utility/include/LoggingFwd.h>

#include <memory>
#include <string>
#include <filesystem>
#include <vector>
#include <functional>

namespace api
{
    class DecoderFacade
    {
        struct Internal;
        std::shared_ptr<Internal> internal;

        template <typename T>
        void decodeFile(std::filesystem::path filePath, std::function<void(T &&, std::string)> transformer);

    public:
        DecoderFacade(utility::LoggerFactory &loggerFactory);

        DecoderFacade(utility::LoggerFactory &loggerFactory, std::filesystem::path publicKeyFilePath);

        std::string decodeRawBase64ToJson(std::string base64RawData);

        std::vector<std::string> decodeFileToJson(std::filesystem::path filePath);

        std::vector<std::vector<std::uint8_t>> decodeFileToRawBytes(std::filesystem::path filePath);

        std::vector<std::string> decodeFileToRawBase64(std::filesystem::path filePath);
    };
}
