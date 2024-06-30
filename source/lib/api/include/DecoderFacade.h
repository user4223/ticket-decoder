#pragma once

#include <lib/utility/include/DebugController.h>
#include <lib/utility/include/LoggingFwd.h>

#include <lib/io/api/include/InputElement.h>
#include <lib/io/api/include/LoadResult.h>
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
        std::shared_ptr<Options> options; // unfortunately, forward declaration works with shared_ptr only, but not with unique_ptr

        /* Use DecoderFacade::create for creation instead of this ctor!
         */
        DecoderFacadeBuilder(::utility::LoggerFactory &loggerFactory);

    public:
        friend DecoderFacade;
        DecoderFacadeBuilder(DecoderFacadeBuilder const &) = delete;
        DecoderFacadeBuilder(DecoderFacadeBuilder &&) = delete;
        DecoderFacadeBuilder &operator=(DecoderFacadeBuilder const &) = delete;
        DecoderFacadeBuilder &operator=(DecoderFacadeBuilder &&) = delete;

        DecoderFacadeBuilder &withInputElementVisitor(std::function<void(io::api::InputElement const &)> visitor);

        DecoderFacadeBuilder &withDetectionResultVisitor(std::function<void(dip::detection::api::Result const &)> visitor);

        DecoderFacadeBuilder &withDecodingResultVisitor(std::function<void(barcode::api::Result const &)> visitor);

        DecoderFacadeBuilder &withAsynchronousLoad(bool loadAsynchronously);

        DecoderFacadeBuilder &withFailOnDecodingError(bool failOnDecodingError);

        DecoderFacadeBuilder &withFailOnInterpretationError(bool failOnInterpretationError);

        DecoderFacadeBuilder &withReaderDpi(int dpi);

        DecoderFacadeBuilder &withDetectorType(dip::detection::api::DetectorType type);

        DecoderFacadeBuilder &withPublicKeyFile(std::filesystem::path publicKeyFilePath);

        DecoderFacadeBuilder &withPureBarcode(bool pureBarcode);

        DecoderFacadeBuilder &withLocalBinarizer(bool localBinarizer);

        DecoderFacadeBuilder &withImageRotation(int rotationDegree);

        DecoderFacadeBuilder &withImageScale(unsigned int scalePercent);

        DecoderFacadeBuilder &withImageSplit(std::string split);

        DecoderFacadeBuilder &withImageFlipping(unsigned int flippingMode);

        DecoderFacadeBuilder &withJsonIndent(int indent);

        DecoderFacadeBuilder &withClassifierFile(std::filesystem::path classifierFile);

        DecoderFacade build();
    };

    class DecoderFacade
    {
        ::utility::Logger logger;
        struct Internal;
        std::shared_ptr<Internal> internal; // unfortunately, forward declaration works with shared_ptr only, but not with unique_ptr
        DecoderFacadeBuilder::Options const &options;

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

        static DecoderFacadeBuilder create(::utility::LoggerFactory &loggerFactory);

        ::utility::DebugController &getDebugController();

        std::string decodeRawFileToJson(std::filesystem::path filePath);

        std::string decodeRawBytesToJson(std::vector<std::uint8_t> rawData, std::string origin = "");

        std::string decodeRawBase64ToJson(std::string base64RawData, std::string origin = "");

        std::vector<std::string> decodeFileToJson(std::filesystem::path filePath);

        std::vector<std::vector<std::uint8_t>> decodeFileToRawBytes(std::filesystem::path filePath);

        std::vector<std::string> decodeFileToRawBase64(std::filesystem::path filePath);

        io::api::LoadResult load(std::filesystem::path path);
    };
}
