#pragma once

#include <lib/utility/include/LoggingFwd.h>

#include <lib/io/api/include/InputElement.h>
#include <lib/dip/detection/api/include/DetectorType.h>
#include <lib/dip/detection/api/include/Result.h>
#include <lib/barcode/api/include/Result.h>

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

        /* Use DecoderFacade::create for creation instead of this ctor!
         */
        DecoderFacadeBuilder(utility::LoggerFactory &loggerFactory);

    public:
        friend DecoderFacade;
        DecoderFacadeBuilder(DecoderFacadeBuilder const &) = delete;
        DecoderFacadeBuilder(DecoderFacadeBuilder &&) = delete;
        DecoderFacadeBuilder &operator=(DecoderFacadeBuilder const &) = delete;
        DecoderFacadeBuilder &operator=(DecoderFacadeBuilder &&) = delete;

        DecoderFacadeBuilder &withInputElementVisitor(std::function<void(io::api::InputElement const &)> visitor);

        DecoderFacadeBuilder &withDetectionResultVisitor(std::function<void(dip::detection::api::Result const &)> visitor);

        DecoderFacadeBuilder &withDecodingResultVisitor(std::function<void(barcode::api::Result const &)> visitor);

        DecoderFacadeBuilder &withFailOnDecodingError(bool failOnDecodingError);

        DecoderFacadeBuilder &withFailOnInterpretationError(bool failOnInterpretationError);

        DecoderFacadeBuilder &withDetectorType(dip::detection::api::DetectorType type);

        DecoderFacadeBuilder &withPublicKeyFile(std::filesystem::path publicKeyFilePath);

        DecoderFacadeBuilder &withPureBarcode(bool pureBarcode);

        DecoderFacadeBuilder &withLocalBinarizer(bool localBinarizer);

        DecoderFacadeBuilder &withImageRotation(int rotation);

        DecoderFacadeBuilder &withImageSplit(std::string split);

        DecoderFacadeBuilder &withJsonIndent(int indent);

        DecoderFacade build();
    };

    class DecoderFacade
    {
        utility::Logger logger;
        struct Internal;
        std::shared_ptr<Internal> internal;

        template <typename T>
        void decodeFiles(std::filesystem::path path, std::function<void(T &&, std::string)> transformer);

        std::string interpretRawBytes(std::vector<std::uint8_t> bytes, std::string origin);

        /* Use DecoderFacade::create for creation instead of this ctor!
         */
        DecoderFacade(std::shared_ptr<DecoderFacadeBuilder::Options> options);

    public:
        friend DecoderFacadeBuilder;
        DecoderFacade(DecoderFacade const &) = delete;
        DecoderFacade(DecoderFacade &&) = delete;
        DecoderFacade &operator=(DecoderFacade const &) = delete;
        DecoderFacade &operator=(DecoderFacade &&) = delete;

        static DecoderFacadeBuilder create(utility::LoggerFactory &loggerFactory);

        std::string decodeRawBytesToJson(std::vector<std::uint8_t> rawData, std::string origin = "");

        std::string decodeRawBase64ToJson(std::string base64RawData, std::string origin = "");

        std::vector<std::string> decodeFileToJson(std::filesystem::path filePath);

        std::vector<std::vector<std::uint8_t>> decodeFileToRawBytes(std::filesystem::path filePath);

        std::vector<std::string> decodeFileToRawBase64(std::filesystem::path filePath);
    };
}
