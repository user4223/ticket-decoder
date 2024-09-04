#pragma once

#include <lib/infrastructure/include/ParameterSupplier.h>
#include <lib/infrastructure/include/ParameterCollector.h>

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
#include <iterator>

namespace utility
{
    class DebugController;
}

namespace dip::filtering
{
    class PreProcessor;
}

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

        DecoderFacadeBuilder &withPreProcessorResultVisitor(std::function<void(io::api::InputElement const &)> visitor);

        DecoderFacadeBuilder &withDetectorResultVisitor(std::function<void(dip::detection::api::Result const &)> visitor);

        DecoderFacadeBuilder &withDecoderResultVisitor(std::function<void(barcode::api::Result const &)> visitor);

        DecoderFacadeBuilder &withInterpreterResultVisitor(std::function<void(std::string const &)> visitor);

        DecoderFacadeBuilder &withAsynchronousLoad(bool loadAsynchronously);

        DecoderFacadeBuilder &withFailOnDecoderError(bool failOnDecoderError);

        DecoderFacadeBuilder &withFailOnInterpreterError(bool failOnInterpreterError);

        DecoderFacadeBuilder &withReaderDpi(int dpi);

        DecoderFacadeBuilder &withDetector(dip::detection::api::DetectorType type);

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

    class DecoderFacade : public infrastructure::ParameterSupplier, public infrastructure::ParameterCollector
    {
        ::utility::Logger logger;
        struct Internal;
        std::shared_ptr<Internal> internal;
        DecoderFacadeBuilder::Options const &options;

        template <typename T>
        void decodeImage(io::api::InputElement image, std::function<void(T &&, std::string)> transformer);

        template <typename T>
        void decodeImageFiles(std::filesystem::path path, std::function<void(T &&, std::string)> transformer);

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

        dip::filtering::PreProcessor &getPreProcessor();

        ::utility::DebugController &getDebugController();

        /* Load all supported elements synchronously/asynchronously from given file/directory
         */
        io::api::LoadResult loadSupportedFiles(std::filesystem::path path);

        /* Aztec code detector handling
         */
        std::vector<dip::detection::api::DetectorType> getSupportetDetectorTypes() const;

        std::string setDetector(dip::detection::api::DetectorType type);

        dip::detection::api::DetectorType getDetector() const;

        /* Raw uic918 input from file, byte-array or base64-string to json
         */
        std::string decodeRawFileToJson(std::filesystem::path filePath);

        std::string decodeRawBytesToJson(std::vector<std::uint8_t> rawData, std::string origin = "");

        std::string decodeRawBase64ToJson(std::string base64RawData, std::string origin = "");

        /* Barcodes from image or PDF input file/directory to json, raw byte-array or raw base64-string
         */
        std::vector<std::pair<std::string, std::string>> decodeImageFilesToJson(std::filesystem::path path);

        std::vector<std::pair<std::string, std::vector<std::uint8_t>>> decodeImageFilesToRawBytes(std::filesystem::path path);

        std::vector<std::pair<std::string, std::string>> decodeImageFilesToRawBase64(std::filesystem::path path);

        /* Pre-loaded image data as input-element to json
         */
        std::vector<std::string> decodeImageToJson(io::api::InputElement image);

        ParameterTypeList supplyParameters() const;
    };
}
