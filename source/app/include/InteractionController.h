#pragma once

#include "lib/infrastructure/include/ParameterCollector.h"
#include "lib/infrastructure/include/ContextFwd.h"

#include "lib/input/api/include/InputElement.h"
#include "lib/output/api/include/SinkManager.h"

#include "lib/detector/api/include/Result.h"

#include "lib/decoder/api/include/Result.h"

#include "lib/utility/include/FrameRate.h"
#include "lib/utility/include/Logger.h"

#include <opencv2/core.hpp>

#include <map>
#include <string>
#include <memory>
#include <functional>
#include <vector>

// TODO Move into ui module and name it InteractionController (maybe) and integrate key handler here as well
class InteractionController : public infrastructure::ParameterCollector
{
    ::utility::Logger logger;
    io::api::SinkManager sinkManager;
    std::unique_ptr<io::api::Writer> writer;
    utility::FrameRate frameRate;

    bool inputChanged = true;
    std::optional<bool> validated;

    std::optional<std::function<cv::Mat()>> fallbackOutputImageSupplier;
    cv::Mat outputImage;
    std::vector<std::string> textLines;

public:
    bool overlayText = false;
    bool overlayImage = true;
    int dumpResults = 1;

    InteractionController(infrastructure::Context &context, io::api::SinkManager sinkManager);

    void reset(bool inputChanged);

    void handlePreProcessorResult(io::api::InputElement const &preProcessorResult);

    void handleDetectorResult(dip::detection::api::Result const &result);

    void handleDecoderResult(barcode::api::Result const &result);

    void handleInterpreterResult(std::string const &result);

    cv::Mat compose();
};
