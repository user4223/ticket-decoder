#include "../include/AztecDecoder.h"

#include "ZXing/ReadBarcode.h"
#include "ZXing/HybridBinarizer.h"
#include "ZXing/GlobalHistogramBinarizer.h"
#include "ZXing/GenericLuminanceSource.h"
#include "ZXing/DecoderResult.h"
#include "ZXing/aztec/AZReader.h"
#include "ZXing/aztec/AZDetector.h"
#include "ZXing/aztec/AZDecoder.h"
#include "ZXing/aztec/AZDetectorResult.h"

struct AztecDecoder::Internal
{
  std::shared_ptr<ZXing::LuminanceSource const> source;
  std::shared_ptr<ZXing::BitMatrix const> matrix;
  ZXing::Aztec::DetectorResult detectorResult;
};

AztecDecoder::AztecDecoder(std::shared_ptr<Internal> i) : internal(i) {}

std::unique_ptr<Decoder> AztecDecoder::create(cv::Mat const &image)
{
  auto internal = std::make_shared<Internal>();
  internal->source = std::make_shared<ZXing::GenericLuminanceSource>(image.cols, image.rows, image.data, image.step);
  // Our image is already binarized, so we could use a dummy binarizer here
  internal->matrix = ZXing::GlobalHistogramBinarizer{internal->source}.getBlackMatrix();
  // internal->matrix = ZXing::HybridBinarizer{internal->source}.getBlackMatrix();
  return std::unique_ptr<Decoder>{new AztecDecoder(std::move(internal))};
}

bool AztecDecoder::detect()
{
  if (internal->matrix == nullptr)
  {
    return false;
  }

  internal->detectorResult = ZXing::Aztec::Detector::Detect(*(internal->matrix), false, false);
  return internal->detectorResult.isValid();
}

std::tuple<bool, std::vector<std::uint8_t>> AztecDecoder::decode()
{
  if (!internal->detectorResult.isValid())
  {
    return {false, {}};
  }

  auto const decodeResult = ZXing::Aztec::Decoder::Decode(internal->detectorResult, "UTF8");
  return {decodeResult.isValid(), std::vector<std::uint8_t>(decodeResult.rawBytes())};
}