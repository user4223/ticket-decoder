
#include "../include/DecoderFacade.h"

#include "lib/utility/include/Logging.h"
#include "lib/utility/include/Base64.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"
#include "lib/io/api/include/Utility.h"

#include "lib/dip/detection/api/include/Detector.h"
#include "lib/dip/filtering/include/PreProcessor.h"

#include "lib/barcode/api/include/Decoder.h"

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/uic918/api/include/SignatureChecker.h"

namespace api
{
    struct DecoderFacadeBuilder::Options
    {
        utility::LoggerFactory &loggerFactory;
        std::optional<dip::detection::api::DetectorType> detectorType;
        std::optional<std::filesystem::path> publicKeyFilePath;
        std::optional<bool> pureBarcode;
        std::optional<bool> localBinarizer;
        std::optional<int> imageRotation;
        std::optional<std::string> imageSplit;

        Options(utility::LoggerFactory &lf) : loggerFactory(lf) {}
    };

    DecoderFacadeBuilder::DecoderFacadeBuilder(utility::LoggerFactory &loggerFactory)
        : options(std::make_shared<Options>(loggerFactory))
    {
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withDetectorType(dip::detection::api::DetectorType type)
    {
        options->detectorType = std::make_optional(type);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withPublicKeyFile(std::filesystem::path publicKeyFilePath)
    {
        options->publicKeyFilePath = std::make_optional(publicKeyFilePath);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withPureBarcode(bool pure)
    {
        options->pureBarcode = std::make_optional(pure);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withLocalBinarizer(bool localBinarizer)
    {
        options->localBinarizer = std::make_optional(localBinarizer);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withImageRotation(int rotation)
    {
        options->imageRotation = std::make_optional(rotation);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withImageSplit(std::string split)
    {
        options->imageSplit = std::make_optional(split);
        return *this;
    }

    DecoderFacade DecoderFacadeBuilder::build()
    {
        return DecoderFacade(*options);
    }

    DecoderFacadeBuilder DecoderFacade::create(::utility::LoggerFactory &loggerFactory)
    {
        return DecoderFacadeBuilder(loggerFactory);
    }

    struct DecoderFacade::Internal
    {
        utility::LoggerFactory &loggerFactory;
        io::api::Loader const loader;
        dip::filtering::PreProcessor const preProcessor;
        std::unique_ptr<dip::detection::api::Detector> const detector;
        std::unique_ptr<barcode::api::Decoder> const decoder;
        std::unique_ptr<uic918::api::SignatureChecker> const signatureChecker;
        std::unique_ptr<uic918::api::Interpreter> const interpreter;

        Internal(DecoderFacadeBuilder::Options const &options)
            : loggerFactory(options.loggerFactory),
              loader(loggerFactory, io::api::Reader::create(loggerFactory)),
              preProcessor(dip::filtering::PreProcessor::create(
                  loggerFactory,
                  options.imageRotation.value_or(0),
                  options.imageSplit.value_or("11"))),
              detector(dip::detection::api::Detector::create(
                  loggerFactory,
                  options.detectorType.value_or(dip::detection::api::DetectorType::NOP_FORWARDER))),
              decoder(barcode::api::Decoder::create(
                  loggerFactory,
                  {options.pureBarcode.value_or(false), options.localBinarizer.value_or(false)})),
              signatureChecker(
                  options.publicKeyFilePath
                      ? uic918::api::SignatureChecker::create(loggerFactory, *options.publicKeyFilePath)
                      : uic918::api::SignatureChecker::createDummy(loggerFactory)),
              interpreter(uic918::api::Interpreter::create(loggerFactory, *signatureChecker))
        {
        }

        std::optional<std::string> interpret(std::vector<std::uint8_t> const &payload, std::string origin)
        {
            return interpreter->interpret(payload, origin, 3);
        }
    };

    template <typename T>
    void DecoderFacade::decodeFile(std::filesystem::path filePath, std::function<void(T &&, std::string)> transformer)
    {
        if (!io::api::utility::areFiles({filePath}))
        {
            throw std::runtime_error("Given path is not a file: " + filePath.string());
        }
        internal->loader.load(filePath, [&](auto &&inputElement)
                              {
                    auto source = internal->preProcessor.get(std::move(inputElement));
                    if (!source.isValid())
                    {
                        return;
                    }
                    auto const barcodes = internal->detector->detect(source.getImage());
                    barcodes.for_each([&](auto const &contourDescriptor)
                                    {
                                        auto decoderResult = internal->decoder->decode(contourDescriptor);
                                        if (!decoderResult.isDecoded())
                                        {
                                            return;
                                        }
                                        transformer(std::move(decoderResult), source.getAnnotation());
                                    }); });
    }

    DecoderFacade::DecoderFacade(DecoderFacadeBuilder::Options const &options)
        : internal(std::make_shared<Internal>(options))
    {
    }

    std::string DecoderFacade::decodeRawBase64ToJson(std::string base64RawData)
    {
        auto const bytes = ::utility::base64::decode(base64RawData);
        auto const json = internal->interpret(bytes, "");
        if (!json)
        {
            throw std::runtime_error("No UIC918 structured data found, version not matching or implemented, or interpretation failed");
        }
        return *json;
    }

    std::vector<std::string> DecoderFacade::decodeFileToJson(std::filesystem::path filePath)
    {
        auto result = std::vector<std::string>{};
        decodeFile<barcode::api::Result>(filePath, [&](auto &&decoderResult, auto origin)
                                         { result.emplace_back(internal->interpret(decoderResult.payload, origin).value_or("{}")); });
        return result;
    }

    std::vector<std::vector<std::uint8_t>> DecoderFacade::decodeFileToRawBytes(std::filesystem::path filePath)
    {
        auto result = std::vector<std::vector<std::uint8_t>>{};
        decodeFile<barcode::api::Result>(filePath, [&](auto &&decoderResult, auto origin)
                                         { result.emplace_back(decoderResult.payload); });
        return result;
    }

    std::vector<std::string> DecoderFacade::decodeFileToRawBase64(std::filesystem::path filePath)
    {
        auto result = std::vector<std::string>{};
        decodeFile<barcode::api::Result>(filePath, [&](auto &&decoderResult, auto origin)
                                         { result.emplace_back(utility::base64::encode(decoderResult.payload)); });
        return result;
    }
}
