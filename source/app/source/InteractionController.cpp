
#include "../include/InteractionController.h"

#include "lib/infrastructure/include/Context.h"

#include "lib/utility/include/Logging.h"
#include "lib/utility/include/JsonSupplier.h"

#include "lib/io/api/include/InputElement.h"

#include "lib/dip/include/Transform.h"

#include "lib/decoder/api/include/Utility.h"

#include "lib/dip/include/Text.h"
#include "lib/dip/include/Shape.h"
#include "lib/dip/include/Color.h"
#include "lib/dip/include/Image.h"

#include <nlohmann/json.hpp>

InteractionController::InteractionController(infrastructure::Context &c, io::api::SinkManager sm)
    : logger(CREATE_LOGGER(c.getLoggerFactory())),
      sinkManager(std::move(sm))
{
    addParameterSupplier(frameRate);
}

void InteractionController::reset(bool ic)
{
    inputChanged = ic;
    validated = std::nullopt;
    fallbackOutputImageSupplier = std::nullopt;
    frameRate.update();
    textLines = {};
}

void InteractionController::handlePreProcessorResult(io::api::InputElement const &preProcessorResult)
{
    fallbackOutputImageSupplier = std::make_optional([&]()
                                                     { return preProcessorResult.getImage(); });

    if (dumpResults > 0 && inputChanged)
    {
        writer = sinkManager.get(preProcessorResult);
    }
}

void InteractionController::handleDetectorResult(dip::detection::api::Result const &result)
{
    if (overlayImage && result.debugImage)
    {
        outputImage = dip::filtering::toColor(result.debugImage->clone());
    }
    else
    {
        outputImage = dip::filtering::toColor(fallbackOutputImageSupplier.value()());
    }

    auto const outputContours = result.debugContours.value_or(result.contours);
    std::for_each(outputContours.begin(), outputContours.end(),
                  [&](auto const &descriptor)
                  {
                      if (overlayImage)
                      {
                          auto const roi = cv::Rect(
                              std::max(descriptor.image.cols - descriptor.square.width, 0) / 2,
                              std::max(descriptor.image.rows - descriptor.square.height, 0) / 2,
                              descriptor.square.width, descriptor.square.height);
                          auto const intersection = cv::Rect({}, descriptor.image.size()) & roi;
                          dip::utility::copyTo(outputImage, descriptor.image(intersection), descriptor.square);
                      }
                      dip::utility::drawRedShape(outputImage, descriptor.contour);
                      dip::utility::drawBlueText(outputImage, descriptor.evaluateAnnotations());
                  });
}

void InteractionController::handleDecoderResult(barcode::api::Result const &result)
{
    dip::utility::drawShape(outputImage, result.box, barcode::api::getDrawProperties(result.level));

    if (dumpResults > 1 && writer && inputChanged)
    {
        if (result.isDecoded())
        {
            writer->write(result.payload);
        }

        if (!result.image.empty())
        {
            auto postfix = std::string{};
            switch (result.level)
            {
            case barcode::api::Level::Decoded:
                postfix += "decoded";
                break;
            case barcode::api::Level::Detected:
                postfix += "detected";
                break;
            default:
                postfix += "failed";
                break;
            }
            writer->write(result.image, postfix);
        }
    }
}

void InteractionController::handleInterpreterResult(std::string const &result)
{
    auto const json = nlohmann::json::parse(result);
    validated = ::utility::getString(json, "validated").value_or("false") == "true";

    if (overlayText)
    {
        auto stream = std::stringstream{result};
        auto counter = 0u;
        for (std::string line; std::getline(stream, line, '\n') && counter < 40; ++counter)
        {
            textLines.push_back(line);
        }
    }

    if (dumpResults > 0 && writer && inputChanged)
    {
        auto postfix = std::string();
        ::utility::ifNode(json, "records")([&](auto const &records)
                                           {
            if (!::utility::ifString(records, "U_HEAD", "uniqueTicketKey")([&](auto const &ticketKey)
                                                                           { postfix += ticketKey; }))
            {
                ::utility::ifString(records, "U_FLEX", "issuingDetail", "issuerPNR")([&](auto const &issuerPNR)
                                                                                     { postfix += issuerPNR; });
            } });
        LOG_INFO(logger) << "Dumped json to file: " << writer->write(result, postfix);
    }
}

cv::Mat InteractionController::compose()
{
    dip::utility::drawBlueText(outputImage, dip::utility::getDimensionAnnotations(outputImage));
    dip::utility::drawShape(outputImage, cv::Rect(outputImage.cols - 60, 50, 30, 30), dip::utility::ShapeProperties{dip::utility::colorOf(validated), -1});

    auto lineCount = dip::utility::drawRedText(outputImage, cv::Point(5, 35), 35, 280, getParameters());
    lineCount += dip::utility::drawRedText(outputImage, cv::Point(5, (lineCount + 1) * 35), 35, textLines);

    return std::move(outputImage);
}
