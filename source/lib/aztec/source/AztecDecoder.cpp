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

#include <locale>
#include <codecvt>

struct AztecDecoder::Internal
{
  std::shared_ptr<ZXing::LuminanceSource const> source;
  std::shared_ptr<ZXing::BitMatrix const> matrix;
  ZXing::Aztec::DetectorResult detectorResult;
  ZXing::DecoderResult decoderResult;
  bool detectionFinished = false;
  bool pure = false;
};

AztecDecoder::AztecDecoder(std::shared_ptr<Internal> i) : internal(i) {}

std::unique_ptr<BarcodeDecoder> AztecDecoder::create(ContourDescriptor const &contourDescriptor, bool const pure)
{
  auto internal = std::make_shared<Internal>();
  auto const &image = contourDescriptor.image;
  if (!image.empty())
  {
    internal->source = std::make_shared<ZXing::GenericLuminanceSource>(image.cols, image.rows, image.data, image.step);
    // Our image is already binarized, so we could use a dummy binarizer here
    internal->matrix = ZXing::GlobalHistogramBinarizer{internal->source}.getBlackMatrix();
    // internal->matrix = ZXing::HybridBinarizer{internal->source}.getBlackMatrix();
  }
  internal->pure = pure;
  return std::unique_ptr<BarcodeDecoder>{new AztecDecoder(std::move(internal))};
}

BarcodeDecodingResult AztecDecoder::decode(ContourDescriptor const &contourDescriptor, bool const pure)
{
  return AztecDecoder::create(contourDescriptor, pure)->decode();
}

BarcodeDecodingLevel AztecDecoder::detect()
{
  if (internal->matrix == nullptr)
  {
    return BarcodeDecodingLevel::Unknown;
  }

  internal->detectorResult = ZXing::Aztec::Detector::Detect(*(internal->matrix), false, internal->pure);
  internal->detectionFinished = true;
  return internal->detectorResult.isValid() ? BarcodeDecodingLevel::Detected : BarcodeDecodingLevel::Unknown;
}

BarcodeDecodingResult AztecDecoder::decode()
{
  if (!internal->detectionFinished)
  {
    detect();
  }

  if (!internal->detectorResult.isValid())
  {
    return BarcodeDecodingResult{BarcodeDecodingLevel::Unknown};
  }

  internal->decoderResult = ZXing::Aztec::Decoder::Decode(internal->detectorResult, "ISO-8859-1"); // Actually it should be UTF8
  if (!internal->decoderResult.isValid())
  {
    return BarcodeDecodingResult{BarcodeDecodingLevel::Detected};
  }

  auto buffer = std::vector<std::uint8_t>(internal->decoderResult.text().size());
  std::transform(internal->decoderResult.text().begin(),
                 internal->decoderResult.text().end(),
                 buffer.begin(),
                 [](std::wstring::value_type const &v)
                 { return (uint8_t)v; });
  return BarcodeDecodingResult{BarcodeDecodingLevel::Decoded, std::move(buffer)};
}