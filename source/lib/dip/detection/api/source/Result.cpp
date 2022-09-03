
#include "../include/Result.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace dip::detection::api
{
  Result::Result(std::vector<Descriptor> &&d)
      : Result(std::move(d), std::optional<cv::Mat>{}, std::optional<std::vector<Descriptor>>{}) {}

  Result::Result(std::vector<Descriptor> &&d, std::optional<cv::Mat> &&di, std::optional<std::vector<Descriptor>> dd)
      : contours(std::move(d)), debugImage(std::move(di)), debugContours(std::move(dd)) {}

  static auto const blue = cv::Scalar(255, 0, 0);
  static auto const red = cv::Scalar(0, 0, 255);
  static auto const yellow = cv::Scalar(0, 255, 255);

  cv::Mat Result::visualize(cv::Mat &&input_) const
  {
    auto input = debugImage.value_or(input_);
    auto destination = input.channels() == 3 ? std::move(input) : [input = std::move(input)]()
    {
      cv::Mat transformed;
      cv::cvtColor(input, transformed, cv::COLOR_GRAY2RGB);
      return transformed;
    }();

    auto const coordinateThickness = 2;
    cv::putText(destination, "0x0", cv::Point(0, 25), cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness);
    cv::putText(destination, std::to_string(destination.cols) + "x0", cv::Point(destination.cols - 125, 25), cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness);
    cv::putText(destination, std::to_string(destination.cols) + "x" + std::to_string(destination.rows), cv::Point(destination.cols - 180, destination.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness);
    cv::putText(destination, "0x" + std::to_string(destination.rows), cv::Point(0, destination.rows - 10), cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness);

    auto const &contousToVisualize = debugContours.value_or(contours);
    if (!contousToVisualize.empty())
    {
      std::for_each(contousToVisualize.begin(), contousToVisualize.end(), [&](auto const &d)
                    {
                    if (d.contour.empty()) 
                    {
                      return;
                    }

                    if (!d.image.empty())
                    {
                      auto const &part = d.image.channels() == 3 ? d.image : [&d]()
                      {
                        cv::Mat transformed;
                        cv::cvtColor(d.image, transformed, cv::COLOR_GRAY2RGB);
                        return transformed;
                      }();
                      part(cv::Rect(0, 0, d.square.width, d.square.height)).copyTo(destination(d.square));
                    }

                    cv::polylines(destination, d.contour, true, red, 2);
                    cv::rectangle(destination, d.square.tl(), d.square.br(), red, 2);

                    std::for_each(d.annotators.begin(), d.annotators.end(), [&](auto const annotator)
                                  {
                                    auto const [position, text] = annotator(d);
                                    cv::putText(destination, text, position, cv::FONT_HERSHEY_SIMPLEX, 1., blue, coordinateThickness); 
                                  }); });
    }

    return destination;
  }
}
