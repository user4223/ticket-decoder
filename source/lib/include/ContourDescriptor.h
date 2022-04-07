
#include <opencv2/core.hpp>

#include <vector>
#include <string>

struct ContourDescriptor
{
  using ContourType = std::vector<cv::Point>;

  ContourType contour;
  std::vector<std::string> annotations;

  std::string toString() const;

  static std::vector<ContourDescriptor> fromContours(std::vector<ContourType> &&contours);

  static std::vector<ContourType> toContours(std::vector<ContourDescriptor> &&descriptors);
};