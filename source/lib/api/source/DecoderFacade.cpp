
#include "../include/DecoderFacade.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/utility/include/Logging.h"
#include "lib/utility/include/DebugController.h"
#include "lib/utility/include/Base64.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"
#include "lib/io/api/include/Utility.h"

#include "lib/dip/filtering/include/PreProcessor.h"
#include "lib/dip/detection/api/include/Detector.h"

#include "lib/barcode/api/include/Decoder.h"

#include "lib/uic918/api/include/Interpreter.h"
#include "lib/uic918/api/include/SignatureChecker.h"

namespace api
{
    class DecoderFacadeBuilder::Options
    {
    public:
        friend DecoderFacadeBuilder;

        std::optional<std::filesystem::path> publicKeyFilePath;
        std::optional<std::filesystem::path> classifierFile;
        std::optional<std::function<void(io::api::InputElement const &)>> preProcessorResultVisitor;
        std::optional<std::function<void(dip::detection::api::Result const &)>> detectorResultVisitor;
        std::optional<std::function<void(barcode::api::Result const &)>> decoderResultVisitor;
        std::optional<std::function<void(std::string const &)>> interpreterResultVisitor;

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

        bool getPureBarcode() const
        {
            return pureBarcode.value_or(barcode::api::DecoderOptions::DEFAULT.pure);
        }

        bool getLocalBinarizer() const
        {
            return localBinarizer.value_or(barcode::api::DecoderOptions::DEFAULT.binarize);
        }

        barcode::api::DecoderOptions getDecoderOptions() const
        {
            return {getPureBarcode(), getLocalBinarizer()};
        }

        bool getFailOnDecodingError() const { return failOnDecodingError.value_or(false); }

        bool getFailOnInterpretationError() const { return failOnInterpretationError.value_or(false); }

        int getJsonIndent() const { return jsonIndent.value_or(3); }

        bool getAsynchronousLoad() const { return asynchronousLoad.value_or(false); }

        void visitPreProcessorResult(io::api::InputElement const &element) const
        {
            if (preProcessorResultVisitor)
            {
                (*preProcessorResultVisitor)(element);
            }
        };

        void visitDetectorResult(dip::detection::api::Result const &result) const
        {
            if (detectorResultVisitor)
            {
                (*detectorResultVisitor)(result);
            }
        }

        void visitDecoderResult(barcode::api::Result const &result) const
        {
            if (decoderResultVisitor)
            {
                (*decoderResultVisitor)(result);
            }
        }

        void visitInterpreterResult(std::string const &result) const
        {
            if (interpreterResultVisitor)
            {
                (*interpreterResultVisitor)(result);
            }
        }
    };

    DecoderFacadeBuilder::DecoderFacadeBuilder(infrastructure::Context &c)
        : context(c),
          options(std::make_shared<Options>())
    {
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withFailOnDecoderError(bool failOnDecodingError)
    {
        options->failOnDecodingError = std::make_optional(failOnDecodingError);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withFailOnInterpreterError(bool failOnInterpretationError)
    {
        options->failOnInterpretationError = std::make_optional(failOnInterpretationError);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withReaderDpi(int dpi)
    {
        options->readerDpi = std::make_optional(dpi);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withDetector(dip::detection::api::DetectorType type)
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

    DecoderFacadeBuilder &DecoderFacadeBuilder::withPreProcessorResultVisitor(std::function<void(io::api::InputElement const &)> visitor)
    {
        options->preProcessorResultVisitor = std::make_optional(visitor);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withDetectorResultVisitor(std::function<void(dip::detection::api::Result const &)> visitor)
    {
        options->detectorResultVisitor = std::make_optional(visitor);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withDecoderResultVisitor(std::function<void(barcode::api::Result const &)> visitor)
    {
        options->decoderResultVisitor = std::make_optional(visitor);
        return *this;
    }

    DecoderFacadeBuilder &DecoderFacadeBuilder::withInterpreterResultVisitor(std::function<void(std::string const &)> visitor)
    {
        options->interpreterResultVisitor = std::make_optional(visitor);
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
        return DecoderFacade(context, options);
    }

    DecoderFacadeBuilder DecoderFacade::create(infrastructure::Context &c)
    {
        return DecoderFacadeBuilder(c);
    }

    class DecoderFacade::Internal
    {
        std::shared_ptr<DecoderFacadeBuilder::Options> options;

    public:
        ::utility::Logger logger;
        ::utility::DebugController &debugController;
        io::api::Loader const loader;
        dip::filtering::PreProcessor preProcessor;
        std::map<dip::detection::api::DetectorType, std::shared_ptr<dip::detection::api::Detector>> const detectors;
        std::shared_ptr<dip::detection::api::Detector> detector;
        std::unique_ptr<barcode::api::Decoder> const decoder;
        std::unique_ptr<uic918::api::SignatureChecker> const signatureChecker;
        std::unique_ptr<uic918::api::Interpreter> const interpreter;

        Internal(infrastructure::Context &context, std::shared_ptr<DecoderFacadeBuilder::Options> o)
            : options(std::move(o)),
              logger(CREATE_LOGGER(context.getLoggerFactory())),
              debugController(context.getDebugController()),
              loader(context.getLoggerFactory(), io::api::Reader::create(
                                                     context.getLoggerFactory(),
                                                     options->getReaderOptions())),
              preProcessor(dip::filtering::PreProcessor::create(
                  context.getLoggerFactory(),
                  options->getPreProcessorOptions())),
              detectors(dip::detection::api::Detector::createAll(
                  context,
                  options->getDetectorOptions())),
              decoder(barcode::api::Decoder::create(
                  context.getLoggerFactory(),
                  debugController,
                  options->getDecoderOptions())),
              signatureChecker(
                  options->publicKeyFilePath
                      ? uic918::api::SignatureChecker::create(context.getLoggerFactory(), *options->publicKeyFilePath)
                      : uic918::api::SignatureChecker::createDummy(context.getLoggerFactory())),
              interpreter(uic918::api::Interpreter::create(context.getLoggerFactory(), *signatureChecker))
        {
        }

        DecoderFacadeBuilder::Options const &getOptions() const { return *options; }
    };

    template <typename T>
    void DecoderFacade::decodeImage(io::api::InputElement inputElement, std::function<void(T &&, std::string)> transformer)
    {
        auto source = internal->preProcessor.get(std::move(inputElement));
        options.visitPreProcessorResult(source);
        if (!source.isValid())
        {
            LOG_INFO(internal->logger) << "Source could not be processed as input, ignoring: " << source.getAnnotation();
            return;
        }
        auto const detectionResult = internal->detector->detect(source.getImage());
        options.visitDetectorResult(detectionResult);
        detectionResult.for_each([&](auto const &contourDescriptor)
                                 {
                                        auto decoderResult = internal->decoder->decode(contourDescriptor);
                                        options.visitDecoderResult(decoderResult);
                                        if (!decoderResult.isDecoded())
                                        {
                                            if (options.getFailOnDecodingError())
                                            {
                                                throw std::runtime_error("Source could not be decoded: " + source.getAnnotation());
                                            }

                                            LOG_INFO(internal->logger) << "Source could not be decoded: " << source.getAnnotation();
                                            return;
                                        }
                                        transformer(std::move(decoderResult), source.getUniquePath()); });
    }

    template <typename T>
    void DecoderFacade::decodeImageFiles(std::filesystem::path path, std::function<void(T &&, std::string)> transformer)
    {
        auto loadHandler = [&, this](auto &&inputElement)
        { decodeImage(std::move(inputElement), transformer); };

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

            LOG_INFO(internal->logger) << "No UIC918 structured data found, version not matching or implemented, or interpretation failed: " << origin;
            options.visitInterpreterResult("{}");
            return "{}";
        }
        options.visitInterpreterResult(*json);
        return *json;
    }

    DecoderFacade::DecoderFacade(infrastructure::Context &c, std::shared_ptr<DecoderFacadeBuilder::Options> o)
        : internal(std::make_shared<Internal>(c, std::move(o))),
          options(internal->getOptions())
    {
        setDetectorType(options.getDetectorType());
        addParameterSupplier(internal->preProcessor);
        addParameterSupplier(internal->debugController);
    }

    dip::filtering::PreProcessor &DecoderFacade::getPreProcessor()
    {
        return internal->preProcessor;
    }

    io::api::LoadResult DecoderFacade::loadSupportedFiles(std::filesystem::path path)
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

    std::vector<dip::detection::api::DetectorType> DecoderFacade::getSupportetDetectorTypes() const
    {
        // std::views not fully supported in clang15, so we use the default way. on upgrade to clang16/17..., switch over to views
        //
        // #if _LIBCPP_STD_VER >= 20
        // auto keys = std::views::keys(internal->detectors);
        // return {keys.begin(), keys.end()};
        // #else
        auto result = std::vector<dip::detection::api::DetectorType>{};
        std::for_each(std::begin(internal->detectors), std::end(internal->detectors), [&](auto const &detector)
                      { result.push_back(detector.second->getType()); });
        return result;
        // #endif
    }

    std::string DecoderFacade::setDetectorType(dip::detection::api::DetectorType type)
    {
        auto detector = internal->detectors.find(type);
        if (detector == internal->detectors.end())
        {
            throw std::runtime_error("Requested detector type not supported or initialized: " + std::to_string((unsigned int)type));
        }
        auto const previous = internal->detector;
        internal->detector = detector->second;
        if (previous)
        {
            replaceParameterSupplier(*previous, *internal->detector);
        }
        else
        {
            addParameterSupplier(*internal->detector);
        }
        return internal->detector->getName();
    }

    dip::detection::api::DetectorType DecoderFacade::getDetectorType() const
    {
        return internal->detector->getType();
    }

    std::string DecoderFacade::decodeRawFileToJson(std::filesystem::path filePath)
    {
        auto const rawUIC918Data = io::api::utility::readBinary(filePath);
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

    std::vector<std::pair<std::string, std::string>> DecoderFacade::decodeImageFilesToJson(std::filesystem::path path)
    {
        auto result = std::vector<std::pair<std::string, std::string>>{};
        decodeImageFiles<barcode::api::Result>(path, [&](auto &&decoderResult, auto origin)
                                               { result.emplace_back(std::make_pair(std::move(origin), interpretRawBytes(std::move(decoderResult.payload), origin))); });
        return result;
    }

    std::vector<std::pair<std::string, std::vector<std::uint8_t>>> DecoderFacade::decodeImageFilesToRawBytes(std::filesystem::path path)
    {
        auto result = std::vector<std::pair<std::string, std::vector<std::uint8_t>>>{};
        decodeImageFiles<barcode::api::Result>(path, [&](auto &&decoderResult, auto origin)
                                               { result.emplace_back(std::make_pair(std::move(origin), std::move(decoderResult.payload))); });
        return result;
    }

    std::vector<std::pair<std::string, std::string>> DecoderFacade::decodeImageFilesToRawBase64(std::filesystem::path path)
    {
        auto result = std::vector<std::pair<std::string, std::string>>{};
        decodeImageFiles<barcode::api::Result>(path, [&](auto &&decoderResult, auto origin)
                                               { result.emplace_back(std::make_pair(std::move(origin), utility::base64::encode(decoderResult.payload))); });
        return result;
    }

    std::vector<std::string> DecoderFacade::decodeImageToJson(io::api::InputElement inputElement)
    {
        auto result = std::vector<std::string>{};
        decodeImage<barcode::api::Result>(std::move(inputElement), [&](auto &&decoderResult, auto origin)
                                          { result.emplace_back(interpretRawBytes(std::move(decoderResult.payload), origin)); });
        return result;
    }

    DecoderFacade::ParameterTypeList DecoderFacade::supplyParameters() const
    {
        return getParameters();
    }
}
