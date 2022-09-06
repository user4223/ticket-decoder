#include "../include/AztecDecoder.h"

#include "ZXing/ReadBarcode.h"

#include "lib/dip/filtering/include/Transform.h"

#include <locale>
#include <codecvt>

namespace barcode::detail
{
  struct AztecDecoder::Internal
  {
    api::Result result;
    const cv::Mat image;
    bool detectionFinished = false;
    ZXing::DecodeHints hints;
    ZXing::Result zresult;

    Internal(api::Result &&result, cv::Mat const &i, bool p)
        : result(std::move(result)),
          image(dip::filtering::scale(dip::filtering::flipX(i), 2.)),
          detectionFinished(false),
          hints(),
          zresult(ZXing::DecodeStatus::NotFound)
    {
      if (image.empty())
      {
        return;
      }

      hints.setFormats(ZXing::BarcodeFormat::Aztec);
      hints.setBinarizer(ZXing::Binarizer::FixedThreshold); // maybe on camera images we prefer LocalAverage
      hints.setCharacterSet("ISO-8859-1");
      hints.setIsPure(p);
      hints.setTryRotate(true);
      hints.setTryHarder(true);

      auto const view = ZXing::ImageView{image.data, image.cols, image.rows, ZXing::ImageFormat::Lum, (int)image.step, 1};
      zresult = ZXing::ReadBarcode(view, hints);
    }
  };

  AztecDecoder::AztecDecoder(unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure)
      : internal(std::make_shared<Internal>(api::Result{id, box}, image, pure)) {}

  api::Level AztecDecoder::detect()
  {
    internal->detectionFinished = true;
    if (!(internal->zresult.position().bottomRight() == ZXing::PointI{}))
    {
      internal->result.level = api::Level::Detected;
    }
    return internal->result.level;
  }

  api::Result AztecDecoder::decode()
  {
    if (!internal->detectionFinished)
    {
      detect();
    }

    if (!internal->zresult.isValid())
    {
      return std::move(internal->result);
    }

    internal->result.level = api::Level::Decoded;

    internal->result.payload = std::vector<std::uint8_t>(internal->zresult.text().size());
    std::transform(internal->zresult.text().begin(),
                   internal->zresult.text().end(),
                   internal->result.payload.begin(),
                   [](std::wstring::value_type const &v)
                   { return (uint8_t)v; });
    return std::move(internal->result);
  }
}
