
#include "../include/DecoderFacade.h"

#include "lib/utility/include/Logging.h"
#include "lib/utility/include/Base64.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"

#include "lib/dip/detection/api/include/Detector.h"
#include "lib/dip/filtering/include/PreProcessor.h"

#include "lib/uic918/api/include/Interpreter.h"

namespace api
{
    struct DecoderFacade::Internal
    {
        io::api::Loader const loader;
        dip::filtering::PreProcessor const preProcessor;
        std::unique_ptr<dip::detection::api::Detector> const detector;
        std::unique_ptr<uic918::api::Interpreter> const interpreter;

        Internal(::utility::LoggerFactory &loggerFactory, dip::detection::api::Detector::Type detectorType)
            : loader(loggerFactory, io::api::Reader::create(loggerFactory)),
              preProcessor(dip::filtering::PreProcessor::create(loggerFactory, 0, "11")),
              detector(dip::detection::api::Detector::create(loggerFactory, detectorType)),
              interpreter(uic918::api::Interpreter::create(loggerFactory))
        {
        }
    };

    DecoderFacade::DecoderFacade(::utility::LoggerFactory &loggerFactory)
        : internal(std::make_shared<Internal>(loggerFactory, dip::detection::api::Detector::Type::NOP_FORWARDER))
    {
    }

    std::string DecoderFacade::decodeUIC918(std::string base64RawData)
    {
        auto const inputBytes = ::utility::base64::decode(base64RawData);
        auto const outputJson = internal->interpreter->interpret(inputBytes, "", 3);
        if (!outputJson)
        {
            throw std::runtime_error("No UIC918 structured data found, version not matching or implemented, or interpretation failed");
        }
        return *outputJson;
    }
}
