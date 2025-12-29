// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/InteractionController.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/infrastructure/include/Logging.h"

#include "lib/utility/include/JsonSupplier.h"

#include "lib/input/api/include/InputElement.h"
#include "lib/output/detail/api/include/Writer.h"

#include "lib/dip/include/Transform.h"

#include "lib/decoder/api/include/DecoderUtility.h"

#include "lib/dip/include/Text.h"
#include "lib/dip/include/Shape.h"
#include "lib/dip/include/Color.h"
#include "lib/dip/include/Image.h"

#include <nlohmann/json.hpp>

InteractionController::InteractionController(infrastructure::Context &c, output::api::SinkManager sm)
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

void InteractionController::handlePreProcessorResult(input::api::InputElement const &preProcessorResult)
{
    fallbackOutputImageSupplier = std::make_optional([&]()
                                                     { return preProcessorResult.getImage(); });

    if (dumpResults > 0 && inputChanged)
    {
        writer = sinkManager.get(preProcessorResult);
    }
}

void InteractionController::handleDetectorResult(detector::api::Result const &result)
{
    if (overlayImage && result.debugImage)
    {
        outputImage = dip::toColor(result.debugImage->clone());
    }
    else
    {
        outputImage = dip::toColor(fallbackOutputImageSupplier.value()());
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
                          dip::copyTo(outputImage, descriptor.image(intersection), descriptor.square);
                      }
                      dip::drawRedShape(outputImage, descriptor.contour);
                      dip::drawBlueText(outputImage, descriptor.evaluateAnnotations());
                  });
}

void InteractionController::handleDecoderResult(decoder::api::Result const &result)
{
    dip::drawShape(outputImage, result.box, decoder::api::getDrawProperties(result.level));

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
            case decoder::api::Level::Decoded:
                postfix += "decoded";
                break;
            case decoder::api::Level::Detected:
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
    dip::drawBlueText(outputImage, dip::getDimensionAnnotations(outputImage));
    dip::drawShape(outputImage, cv::Rect(outputImage.cols - 60, 50, 30, 30), dip::ShapeProperties{dip::colorOf(validated), -1});

    auto lineCount = dip::drawRedText(outputImage, cv::Point(5, 35), 35, 280, getParameters());
    lineCount += dip::drawRedText(outputImage, cv::Point(5, (lineCount + 1) * 35), 35, textLines);

    return std::move(outputImage);
}
