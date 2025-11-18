
#include "../include/SquareDetector.h"

#include "lib/detector/api/include/Descriptor.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include "lib/detector/detail/common/include/DetectorPipe.h"
#include "lib/detector/detail/common/include/FilterPipe.h"
#include "lib/detector/detail/common/include/Utility.h"

#include "lib/dip/include/Transform.h"
#include "lib/dip/include/Color.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace dip::detection::api
{
    SquareDetector::SquareDetector(infrastructure::Context &context, DetectorOptions o)
        : logger(CREATE_LOGGER(context.getLoggerFactory())),
          debugController(context.getDebugController()
                              .define("squareDetector.imageProcessing.step", {0u, 7u, 7u, "sd.ip.step"})
                              .define("squareDetector.imageProcessing.smooth", {3, 7, 11, "sd.ip.smooth"})
                              .define("squareDetector.contourDetector.step", {0u, 18u, 18u, "sd.cd.step"})),
          options(std::move(o))
    {
    }

    std::string SquareDetector::getName() const { return "Square"; }

    DetectorType SquareDetector::getType() const { return DetectorType::SQUARE_DETECTOR; }

    static auto const claheParameters = cv::createCLAHE(1, cv::Size(8, 8));
    static auto const rect3x3Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    static auto const rect5x5Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

    Result SquareDetector::detect(cv::Mat const &input)
    {
        using ip = dip::filtering::pipe::FilterPipe;
        using cd = detail::DetectorPipe;

        auto gray = dip::filtering::toGray(input);
        auto equalized = cv::Mat();
        auto imageDescriptor = ip::filter( // clang-format off
        dip::filtering::pipe::FilterPipeDescriptor::fromImage(gray.clone()),
        debugController.getAs<unsigned int>("squareDetector.imageProcessing.step"),
        {
            ip::equalize(claheParameters), // C ontrast L imited A daptive H istogram E qualization
            ip::cloneInto(equalized),      // Keep a copy of equalized image 4 later
            ip::smooth(debugController.getAs<int>("squareDetector.imageProcessing.smooth")), // Gauss, that's it
            ip::binarize(5, 1),            // Adaptive gaussian threshold binarization
            ip::close(rect3x3Kernel, 1),   // Morph close x times -> remove small dark pixesl
            ip::open(rect3x3Kernel, 3),    // Morph open x times -> join near remaining pixels
        }); // clang-format on

        auto const minimalSize = input.rows * input.cols * (1. / 100.);
        auto pipeDescriptor = cd::filter( // clang-format off
            detail::DetectorPipeDescriptor::fromContours(cd::find(imageDescriptor.image)),
            debugController.getAs<unsigned int>("squareDetector.contourDetector.step"),
            {
                cd::removeIf(cd::areaSmallerThan(minimalSize)),              // Remove small noise
                cd::convexHull(),                                            // Just that
                cd::approximateShapeWith(cd::perimeterTimes(0.10)),          // Remove notches
                cd::removeIf(cd::cornersDoesNotEqual(4)),                    // 4 corners only
                cd::removeIf(cd::sideLengthRatioLessThan(2. / 3.)),          // Square like shapes only
                cd::sortBy(cd::biggestArea()),                               // Biggest first
                cd::removeIfParent(),                                        // Inner squares only
                cd::normalizePointOrder(),                                   // TL, TR, BR, BL
                cd::determineBoundingSquareWith(1.2f),                       // Up-right square with margin
                cd::removeIf(cd::boundingSquareOutsideOf(equalized.size())), // Inside image only
                cd::replaceImagesFrom(equalized),                            // Extract square of each remaining contour
                cd::filterImages({                                           // Filter all extracted contour images again
                    ip::edges(85, 255, 3),                                   // Fine canny edges to make refinement possible
                    /*ip::erode(rect3x3Kernel, 2),*/
                }),
                cd::refineEdges(0.05),                                       // Refine contour corners since there is still huge deviation
                cd::unwarpImagesFrom(gray, 1.10f),                           // Extract and unwarp image to ideal square
                cd::removeIf(cd::emptyImage()),
                cd::filterImages({
                    //ip::smooth(3),
                    //ip::binarize(25, 3),
                    //ip::close(rect3x3Kernel, 1),
                    //ip::open(rect3x3Kernel, 1),
                    ip::resize(0.1f, dip::utility::white),
                }),
                cd::annotateWith({cd::dimensionString(), cd::coordinatesString()}),
            }); // clang-format on

        return Result{
            std::move(pipeDescriptor.contours),
            std::move(imageDescriptor.debugImage),
            std::move(pipeDescriptor.debugContours)};
    }
}
