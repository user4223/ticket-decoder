
#include "../include/DecoderFacade.h"

#include "lib/utility/include/Logging.h"

#include "lib/io/api/include/Reader.h"
#include "lib/io/api/include/Loader.h"

#include "lib/dip/detection/api/include/Detector.h"
#include "lib/dip/filtering/include/PreProcessor.h"

#include "lib/uic918/api/include/Interpreter.h"

namespace api
{
    struct DecoderFacade::Internal
    {
        std::vector<std::shared_ptr<io::api::Reader>> const readers;
        io::api::Loader const loader;
        dip::detection::api::Parameters parameters;
        std::vector<std::shared_ptr<dip::detection::api::Detector>> const detector;
        dip::utility::PreProcessor const preProcessor;
        std::unique_ptr<uic918::api::Interpreter> const interpreter;

        Internal(::utility::LoggerFactory &loggerFactory)
            : readers(io::api::Reader::create(loggerFactory, io::api::ReadOptions{})),
              loader(io::api::Loader(loggerFactory, readers)),
              parameters(),
              detector(dip::detection::api::Detector::createAll(loggerFactory, parameters)),
              preProcessor(dip::utility::PreProcessor::create(loggerFactory, 0, "11")),
              interpreter(uic918::api::Interpreter::create(loggerFactory))
        {
        }
    };

    DecoderFacade::DecoderFacade(::utility::LoggerFactory &loggerFactory)
        : internal(std::make_shared<Internal>(loggerFactory))
    {
    }
}
