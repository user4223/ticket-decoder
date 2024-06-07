#pragma once

#include <lib/utility/include/LoggingFwd.h>

#include <lib/dip/detection/api/include/DetectorType.h>

#include <memory>
#include <string>
#include <filesystem>
#include <vector>
#include <functional>

namespace api
{
    class DecoderFacade;

    class DecoderFacadeBuilder
    {
        struct Options;
        std::shared_ptr<Options> options;

    public:
        friend DecoderFacade;
        DecoderFacadeBuilder(DecoderFacadeBuilder const &) = delete;
        DecoderFacadeBuilder(DecoderFacadeBuilder &&) = delete;
        DecoderFacadeBuilder &operator=(DecoderFacadeBuilder const &) = delete;
        DecoderFacadeBuilder &operator=(DecoderFacadeBuilder &&) = delete;

        DecoderFacadeBuilder(utility::LoggerFactory &loggerFactory);

        DecoderFacadeBuilder &withDetectorType(dip::detection::api::DetectorType type);

        DecoderFacadeBuilder &withPublicKeyFile(std::filesystem::path publicKeyFilePath);

        DecoderFacadeBuilder &withPureBarcode(bool pure);

        DecoderFacadeBuilder &withLocalBinarizer(bool localBinarizer);

        DecoderFacadeBuilder &withImageRotation(int rotation);

        DecoderFacadeBuilder &withImageSplit(std::string split);

        DecoderFacade build();
    };

    class DecoderFacade
    {
        struct Internal;
        std::shared_ptr<Internal> internal;

        template <typename T>
        void decodeFile(std::filesystem::path filePath, std::function<void(T &&, std::string)> transformer);

        DecoderFacade(DecoderFacadeBuilder::Options const &options);

    public:
        friend DecoderFacadeBuilder;
        DecoderFacade(DecoderFacade const &) = delete;
        DecoderFacade(DecoderFacade &&) = delete;
        DecoderFacade &operator=(DecoderFacade const &) = delete;
        DecoderFacade &operator=(DecoderFacade &&) = delete;

        static DecoderFacadeBuilder create(utility::LoggerFactory &loggerFactory);

        std::string decodeRawBase64ToJson(std::string base64RawData);

        std::vector<std::string> decodeFileToJson(std::filesystem::path filePath);

        std::vector<std::vector<std::uint8_t>> decodeFileToRawBytes(std::filesystem::path filePath);

        std::vector<std::string> decodeFileToRawBase64(std::filesystem::path filePath);
    };
}
