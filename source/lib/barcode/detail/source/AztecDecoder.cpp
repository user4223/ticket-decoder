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

#include "lib/dip/filtering/include/Transform.h"

#include <locale>
#include <codecvt>

namespace barcode::detail
{
  struct AztecDecoder::Internal
  {
    api::Result result;
    const cv::Mat image;
    const bool pure;
    const bool mirrored = false;
    bool detectionFinished = false;
    std::shared_ptr<ZXing::LuminanceSource const> source;
    std::shared_ptr<ZXing::BitMatrix const> matrix;
    ZXing::Aztec::DetectorResult detectorResult;
    ZXing::DecoderResult decoderResult;

    Internal(api::Result &&result, cv::Mat const &i, bool p) : result(std::move(result)), image(i), pure(p)
    {
      if (!image.empty())
      {
        source = std::make_shared<ZXing::GenericLuminanceSource>(image.cols, image.rows, image.data, image.step);
        // Depending whether our image is already binarized or not, we could use a dummy binarizer here
        // internal->matrix = ZXing::GlobalHistogramBinarizer{internal->source}.getBlackMatrix();
        matrix = ZXing::HybridBinarizer{source}.getBlackMatrix();
      }
    }
  };

  AztecDecoder::AztecDecoder(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure)
      : internal(std::make_shared<Internal>(api::Result{id, box}, image, pure)) {}

  api::Level AztecDecoder::detect()
  {
    if (internal->matrix == nullptr)
    {
      return api::Level::Unknown;
    }

    internal->detectorResult = ZXing::Aztec::Detector::Detect(
        *(internal->matrix),
        internal->mirrored,
        internal->pure);

    internal->detectionFinished = true;
    internal->result.level = internal->detectorResult.isValid()
                                 ? api::Level::Detected
                                 : api::Level::Unknown;

    return internal->result.level;
  }

  api::Result AztecDecoder::decode()
  {
    if (!internal->detectionFinished)
    {
      detect();
    }

    if (!internal->detectorResult.isValid())
    {
      return std::move(internal->result);
    }

    internal->decoderResult = ZXing::Aztec::Decoder::Decode(internal->detectorResult, "ISO-8859-1"); // Actually it should be UTF8
    if (!internal->decoderResult.isValid())
    {
      // Give it a 2nd chance with 15 degree rotated image, for whatever reason
      auto rotated = dip::filtering::rotate(internal->image, 15.f);
      internal = std::make_shared<Internal>(
          api::Result(internal->result.id, internal->result.box),
          internal->image,
          internal->pure);

      detect();
      if (!internal->detectorResult.isValid())
      {
        return std::move(internal->result);
      }

      internal->decoderResult = ZXing::Aztec::Decoder::Decode(internal->detectorResult, "ISO-8859-1"); // Actually it should be UTF8
      if (!internal->decoderResult.isValid())
      {
        return std::move(internal->result);
      }
    }
    internal->result.level = api::Level::Decoded;

    internal->result.payload = std::vector<std::uint8_t>(internal->decoderResult.text().size());
    std::transform(internal->decoderResult.text().begin(),
                   internal->decoderResult.text().end(),
                   internal->result.payload.begin(),
                   [](std::wstring::value_type const &v)
                   { return (uint8_t)v; });
    return std::move(internal->result);
  }
}
