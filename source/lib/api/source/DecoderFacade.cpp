
#include "../include/DecoderFacade.h"

#include "lib/utility/include/Logging.h"
#include "lib/utility/include/Base64.h"
#include "lib/utility/include/FileSystem.h"
#include "lib/utility/include/DebugController.h"

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
    class DecoderFacadeBuilder::Options
    {
    public:
        friend DecoderFacadeBuilder;

        utility::LoggerFactory &loggerFactory;
        std::optional<std::filesystem::path> publicKeyFilePath;
        std::optional<std::filesystem::path> classifierFile;
        std::optional<std::function<void(io::api::InputElement const &)>> inputElementVisitor;
        std::optional<std::function<void(dip::detection::api::Result const &)>> detectionResultVisitor;
        std::optional<std::function<void(barcode::api::Result const &)>> decodingResultVisitor;

    private:
        std::optional<unsigned int> readerDpi;
        std::optional<int> imageRotation;
        std::optional<unsigned int> imageScale;
        std::optional<std::string> imageSplit;
        std::optional<unsigned int> imageFlipping;
        std::optional<dip::detection::api::DetectorType> detectorType;
        std::optional<bool> pureBarcode;
        std::optional<bool> localBinarizer;
        std::optional<bool> failOnDecodingError;
        std::optional<bool> failOnInterpretationError;
        std::optional<int> jsonIndent;
        std::optional<bool> asynchronousLoad;

    public:
        Options(::utility::LoggerFactory &lf) : loggerFactory(lf) {}

        unsigned int getReaderDpi() const { return readerDpi.value_or(io::api::ReaderOptions::DEFAULT.dpi); }

        io::api::ReaderOptions getReaderOptions() const { return {getReaderDpi()}; }

        int getImageRotation() const { return imageRotation.value_or(dip::filtering::PreProcessorOptions::DEFAULT.rotationDegree); }

        unsigned int getImageScale() const { return imageScale.value_or(dip::filtering::PreProcessorOptions::DEFAULT.scalePercent); }

        std::string getImageSplit() const { return imageSplit.value_or(dip::filtering::PreProcessorOptions::DEFAULT.split); }

        unsigned int getImageFlipping() const { return imageFlipping.value_or(dip::filtering::PreProcessorOptions::DEFAULT.flippingMode); }

        dip::filtering::PreProcessorOptions getPreProcessorOptions() const { return {getImageRotation(), getImageScale(), getImageSplit(), getImageFlipping()}; }

        dip::detection::api::DetectorType getDetectorType() const { return detectorType.value_or(dip::detection::api::DetectorType::NOP_FORWARDER); }

        dip::detection::api::DetectorOptions getDetectorOptions() const
        {
            return classifierFile ? dip::detection::api::DetectorOptions{*classifierFile} : dip::detection::api::DetectorOptions{};
        }

        bool getPureBarcode() const { return pureBarcode.value_or(barcode::api::DecoderOptions::DEFAULT.pure); }

        bool getLocalBinarizer() const { return localBinarizer.value_or(barcode::api::DecoderOptions::DEFAULT.binarize); }

        barcode::api::DecoderOptions getDecoderOptions() const { return {getPureBarcode(), getLocalBinarizer()}; }

        bool getFailOnDecodingError() const { return failOnDecodingError.value_or(false); }

        bool getFailOnInterpretationError() const { return failOnInterpretationError.value_or(false); }

        int getJsonIndent() const { return jsonIndent.value_or(3); }

        bool getAsynchronousLoad() const { return asynchronousLoad.value_or(false); }

        void visitInputElement(io::api::InputElement const &element) const
        {
            if (inputElementVisitor)
            {
                (*inputElementVisitor)(element);
            }
        };

        void visitDetectionResult(dip::detection::api::Result const &result) const
        {
            if (detectionResultVisitor)
            {
                (*detectionResultVisitor)(result);
            }
        }

        void visitDecodingResult(barcode::api::Result const &result) const
        {
            if (decodingResultVisitor)
            {
                (*decodingResultVisitor)(result);
            }
        }
    };

    DecoderFacadeBuilder::DecoderFacadeBuilder(::utility::LoggerFactory &loggerFactory)
        : options(std::make_shared<Options>(loggerFactory))
    {
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withFailOnDecodingError(bool failOnDecodingError)
    {
        options->failOnDecodingError = std::make_optional(failOnDecodingError);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withFailOnInterpretationError(bool failOnInterpretationError)
    {
        options->failOnInterpretationError = std::make_optional(failOnInterpretationError);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withReaderDpi(int dpi)
    {
        options->readerDpi = std::make_optional(dpi);
        return *this;
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

    DecoderFacadeBuilder &DecoderFacadeBuilder::withPureBarcode(bool pureBarcode)
    {
        options->pureBarcode = std::make_optional(pureBarcode);
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

    DecoderFacadeBuilder &DecoderFacadeBuilder::withImageScale(unsigned int scalePercent)
    {
        options->imageScale = std::make_optional(scalePercent);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withImageSplit(std::string split)
    {
        options->imageSplit = std::make_optional(split);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withImageFlipping(unsigned int flippingMode)
    {
        options->imageFlipping = std::make_optional(flippingMode);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withInputElementVisitor(std::function<void(io::api::InputElement const &)> visitor)
    {
        options->inputElementVisitor = std::make_optional(visitor);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withDetectionResultVisitor(std::function<void(dip::detection::api::Result const &)> visitor)
    {
        options->detectionResultVisitor = std::make_optional(visitor);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withDecodingResultVisitor(std::function<void(barcode::api::Result const &)> visitor)
    {
        options->decodingResultVisitor = std::make_optional(visitor);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withAsynchronousLoad(bool loadAsynchronously)
    {
        options->asynchronousLoad = std::make_optional(loadAsynchronously);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withJsonIndent(int indent)
    {
        options->jsonIndent = std::make_optional(indent);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withClassifierFile(std::filesystem::path classifierFile)
    {
        options->classifierFile = std::make_optional(classifierFile);
        return *this;
    }

    DecoderFacade DecoderFacadeBuilder::build()
    {
        return DecoderFacade(options);
    }

    DecoderFacadeBuilder DecoderFacade::create(::utility::LoggerFactory &loggerFactory)
    {
        return DecoderFacadeBuilder(loggerFactory);
    }

    class DecoderFacade::Internal
    {
        std::shared_ptr<DecoderFacadeBuilder::Options> options;
        ::utility::LoggerFactory &loggerFactory;

    public:
        ::utility::DebugController debugController;
        io::api::Loader const loader;
        dip::filtering::PreProcessor const preProcessor;
        std::unique_ptr<dip::detection::api::Detector> const detector;
        std::unique_ptr<barcode::api::Decoder> const decoder;
        std::unique_ptr<uic918::api::SignatureChecker> const signatureChecker;
        std::unique_ptr<uic918::api::Interpreter> const interpreter;

        Internal(std::shared_ptr<DecoderFacadeBuilder::Options> o)
            : options(std::move(o)),
              loggerFactory(options->loggerFactory),
              debugController(),
              loader(loggerFactory, io::api::Reader::create(
                                        loggerFactory,
                                        options->getReaderOptions())),
              preProcessor(dip::filtering::PreProcessor::create(
                  loggerFactory,
                  options->getPreProcessorOptions())),
              detector(dip::detection::api::Detector::create(
                  loggerFactory,
                  debugController,
                  options->getDetectorType(),
                  options->getDetectorOptions())),
              decoder(barcode::api::Decoder::create(
                  loggerFactory,
                  options->getDecoderOptions())),
              signatureChecker(
                  options->publicKeyFilePath
                      ? uic918::api::SignatureChecker::create(loggerFactory, *options->publicKeyFilePath)
                      : uic918::api::SignatureChecker::createDummy(loggerFactory)),
              interpreter(uic918::api::Interpreter::create(loggerFactory, *signatureChecker))
        {
        }

        DecoderFacadeBuilder::Options const &getOptions() const { return *options; }
    };

    template <typename T>
    void DecoderFacade::decodeFiles(std::filesystem::path path, std::function<void(T &&, std::string)> transformer)
    {
        auto loadHandler = [&](auto &&inputElement)
        {
                    options.visitInputElement(inputElement);
                    auto source = internal->preProcessor.get(std::move(inputElement));
                    if (!source.isValid())
                    {
                        LOG_INFO(logger) << "Source could not be processed as input, ignoring: " << source.getAnnotation();
                        return;
                    }
                    auto const detectionResult = internal->detector->detect(source.getImage());
                    options.visitDetectionResult(detectionResult);
                    detectionResult.for_each([&](auto const &contourDescriptor)
                                    {
                                        auto decoderResult = internal->decoder->decode(contourDescriptor);
                                        options.visitDecodingResult(decoderResult);
                                        if (!decoderResult.isDecoded())
                                        {
                                            if (options.getFailOnDecodingError())
                                            {
                                                throw std::runtime_error("Source could not be decoded: " + source.getAnnotation());
                                            }

                                            LOG_INFO(logger) << "Source could not be decoded: " << source.getAnnotation();
                                            return;
                                        }
                                        transformer(std::move(decoderResult), source.getAnnotation());
                                    }); };

        if (options.getAsynchronousLoad())
        {
            internal->loader.loadAsync(path, loadHandler);
        }
        else
        {
            internal->loader.load(path, loadHandler);
        }
    }

    std::string DecoderFacade::interpretRawBytes(std::vector<std::uint8_t> bytes, std::string origin)
    {
        auto const json = internal->interpreter->interpret(bytes, origin, options.getJsonIndent());
        if (!json)
        {
            if (options.getFailOnInterpretationError())
            {
                throw std::runtime_error("No UIC918 structured data found, version not matching or implemented, or interpretation failed:" + origin);
            }

            LOG_INFO(logger) << "No UIC918 structured data found, version not matching or implemented, or interpretation failed: " << origin;
            return "{}";
        }
        return *json;
    }

    DecoderFacade::DecoderFacade(std::shared_ptr<DecoderFacadeBuilder::Options> options)
        : logger(CREATE_LOGGER(options->loggerFactory)),
          internal(std::make_shared<Internal>(options)),
          options(internal->getOptions())
    {
    }

    ::utility::DebugController &DecoderFacade::getDebugController()
    {
        return internal->debugController;
    }

    std::string DecoderFacade::decodeRawFileToJson(std::filesystem::path filePath)
    {
        auto const rawUIC918Data = utility::readBinary(filePath);
        return decodeRawBytesToJson(rawUIC918Data, filePath);
    }

    std::string DecoderFacade::decodeRawBytesToJson(std::vector<std::uint8_t> rawData, std::string origin)
    {
        return interpretRawBytes(std::move(rawData), origin);
    }

    std::string DecoderFacade::decodeRawBase64ToJson(std::string base64RawData, std::string origin)
    {
        return decodeRawBytesToJson(utility::base64::decode(base64RawData), origin);
    }

    std::vector<std::string> DecoderFacade::decodeFileToJson(std::filesystem::path filePath)
    {
        auto result = std::vector<std::string>{};
        decodeFiles<barcode::api::Result>(filePath, [&](auto &&decoderResult, auto origin)
                                          { result.emplace_back(interpretRawBytes(std::move(decoderResult.payload), origin)); });
        return result;
    }

    std::vector<std::vector<std::uint8_t>> DecoderFacade::decodeFileToRawBytes(std::filesystem::path filePath)
    {
        auto result = std::vector<std::vector<std::uint8_t>>{};
        decodeFiles<barcode::api::Result>(filePath, [&](auto &&decoderResult, auto origin)
                                          { result.emplace_back(std::move(decoderResult.payload)); });
        return result;
    }

    std::vector<std::string> DecoderFacade::decodeFileToRawBase64(std::filesystem::path filePath)
    {
        auto result = std::vector<std::string>{};
        decodeFiles<barcode::api::Result>(filePath, [&](auto &&decoderResult, auto origin)
                                          { result.emplace_back(utility::base64::encode(decoderResult.payload)); });
        return result;
    }

    io::api::LoadResult DecoderFacade::load(std::filesystem::path path)
    {
        if (options.getAsynchronousLoad())
        {
            return internal->loader.loadAsync(path);
        }
        else
        {
            return internal->loader.load(path);
        }
    }
}
