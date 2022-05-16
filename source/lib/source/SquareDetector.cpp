
#include "../include/SquareDetector.h"
#include "../include/ContourDescriptor.h"

#include <opencv2/core.hpp> // Reduce include dependencies here

SquareDetector::SquareDetector(Parameters &p) : parameters(p) {}

std::unique_ptr<Detector> SquareDetector::create(Parameters &parameters)
{
    return std::unique_ptr<Detector>{new SquareDetector(parameters)};
}

static auto const claheParameters = cv::createCLAHE(1, cv::Size(8, 8));
static auto const rect3x3Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
static auto const rect5x5Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

DetectionResult SquareDetector::detect(cv::Mat const &input)
{
    using ip = ImageProcessor;
    using cd = ContourDetector;

    cv::Mat equalized;
    auto processed = ip::filter(
        ImageDescriptor::fromImage(ip::toGray(input)),
        {
            ip::equalize(claheParameters), // C ontrast L imited A daptive H istogram E qualization
            ip::cloneInto(equalized),      // Keep a copy of equalized image 4 later
            ip::smooth(7),                 // Gauss, that's it
            ip::binarize(5, 1),            // Adaptive gaussian threshold binarization
            ip::open(rect3x3Kernel, 2),    // Morph open x times
            ip::close(rect3x3Kernel, 1)    // Morph close x times
        });

    auto const minimalSize = input.rows * input.cols * (1. / 100.);
    auto descriptors = cd::filter(
        ContourDescriptor::fromContours(cd::find(processed.image)),
        {
            cd::removeIf(cd::areaSmallerThan(minimalSize)),              // Remove small noise
            cd::convexHull(),                                            // Just that
            cd::approximateShapeWith(cd::perimeterTimes(0.05)),          // Remove notches
            cd::removeIf(cd::cornersDoesNotEqual(4)),                    // 4 corners only
            cd::removeIf(cd::sideLengthRatioLessThan(2. / 3.)),          // Square like shapes only
            cd::sortBy(cd::biggestArea()),                               // Biggest first
            cd::removeIfParent(),                                        // Inner squares only
            cd::normalizePointOrder(),                                   // TL, TR, BR, BL
            cd::determineBoundingSquareWith(1.1f),                       // Up-right square with margin
            cd::removeIf(cd::boundingSquareOutsideOf(equalized.size())), // Inside image only
            cd::extractFrom(equalized),                                  // Copy square
            cd::filterContourImages({
                ip::edges(85, 255, 3),
                /*ip::erode(rect3x3Kernel, 2),*/
            }),
            cd::refineEdges(0.05),           // Refine contour corners since there is still huge deviation
            cd::unwarpFrom(equalized, 1.0f), // Extract and unwarp image to ideal square
            cd::removeIf(cd::emptyImage()),
            cd::filterContourImages({
                ip::binarize(25, 5),
            }),
            cd::annotateWith({cd::dimensionString(), cd::coordinatesString()}),
        });

    return DetectionResult{std::move(processed.image), std::move(descriptors)};
}
