#pragma once

#include "lib/io/api/include/SinkManager.h"
#include "lib/io/api/include/Writer.h"

#include "lib/io/api/include/InputElement.h"
#include "lib/dip/detection/api/include/Result.h"
#include "lib/barcode/api/include/Result.h"

#include "lib/utility/include/FrameRate.h"

#include <opencv2/core.hpp>

#include <map>
#include <string>
#include <optional>
#include <functional>

class OutputComposer
{
    using OutLineType = std::vector<std::pair<std::string, std::string>>;

    io::api::SinkManager sinkManager;
    std::optional<io::api::Writer> writer;
    utility::FrameRate frameRate;

    bool inputChanged = true;
    bool validated = false;

    std::optional<std::function<cv::Mat()>> fallbackOutputImageSupplier;
    cv::Mat outputImage;
    OutLineType outputLines;

public:
    bool overlayText = true;
    bool overlayImage = true;
    bool dumpResults = false;

    OutputComposer(io::api::SinkManager sm);

    void reset(bool ic, std::function<void(OutLineType &)> adder);

    void handlePreProcessorResult(io::api::InputElement const &preProcessorResult);

    void handleDetectorResult(dip::detection::api::Result const &result);

    void handleDecoderResult(barcode::api::Result const &result);

    void handleInterpreterResult(std::string const &result);

    cv::Mat compose();
};
