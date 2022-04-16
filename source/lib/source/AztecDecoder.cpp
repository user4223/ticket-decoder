#include "../include/AztecDecoder.h"

#include "ZXing/ReadBarcode.h"
#include "ZXing/ThresholdBinarizer.h"
#include "ZXing/aztec/AZReader.h"
#include "ZXing/aztec/AZDetector.h"

std::unique_ptr<Decoder> AztecDecoder::create()
{
  return std::unique_ptr<Decoder>{new AztecDecoder()};
}

std::tuple<bool, std::string> AztecDecoder::detect(cv::Mat const &image)
{
  auto const view = ZXing::ImageView{image.data, image.cols, image.rows, ZXing::ImageFormat::Lum};

  auto hints = ZXing::DecodeHints();
  hints.setBinarizer(ZXing::Binarizer::LocalAverage);
  hints.setIsPure(true);
  auto const reader = std::make_unique<ZXing::Aztec::Reader>(hints);
  auto const result = reader->decode(ZXing::ThresholdBinarizer{view, 127});

  return {result.isValid(), ""};
}