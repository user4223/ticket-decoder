#include "../include/AztecDecoder.h"

#include "ZXing/ReadBarcode.h"
#include "ZXing/HybridBinarizer.h"
#include "ZXing/GenericLuminanceSource.h"
#include "ZXing/aztec/AZReader.h"
#include "ZXing/aztec/AZDetector.h"
#include "ZXing/aztec/AZDecoder.h"
#include "ZXing/aztec/AZDetectorResult.h"

std::unique_ptr<Decoder> AztecDecoder::create()
{
  return std::unique_ptr<Decoder>{new AztecDecoder()};
}

std::tuple<bool, std::string> AztecDecoder::detect(cv::Mat const &image)
{
  auto const source = std::make_shared<ZXing::GenericLuminanceSource>(image.cols, image.rows, image.data, image.step);
  auto const matrix = ZXing::HybridBinarizer{source}.getBlackMatrix();
  if (matrix == nullptr)
  {
    return {false, ""};
  }

  auto const detectResult = ZXing::Aztec::Detector::Detect(*matrix, false, false);
  if (!detectResult.isValid())
  {
    return {false, ""};
  }

  return {true, ""};

  // auto const decodeResult = ZXing::Aztec::Decoder::Decode(detectResult, "UTF8");

  // return Result(std::move(decodeResult), std::move(detectResult).position(), BarcodeFormat::Aztec);
}