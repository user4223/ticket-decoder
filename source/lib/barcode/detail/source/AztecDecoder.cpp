#include "../include/AztecDecoder.h"

#include "ZXing/ReadBarcode.h"

#include "lib/dip/filtering/include/Transform.h"

#include "lib/utility/include/Logging.h"

#include <locale>
#include <codecvt>

namespace barcode::detail
{
  struct AztecDecoder::Internal
  {
    api::Result result;
    bool detectionFinished = false;
    ZXing::DecodeHints hints;
    ZXing::Result zresult;

    Internal(api::Result &&r, api::Config config)
        : result(std::move(r)),
          detectionFinished(false),
          hints(),
          zresult()
    {
      if (result.image.empty())
      {
        return;
      }

      hints.setFormats(ZXing::BarcodeFormat::Aztec);
      hints.setBinarizer(config.binarize ? ZXing::Binarizer::LocalAverage : ZXing::Binarizer::BoolCast);
      hints.setCharacterSet(ZXing::CharacterSet::BINARY);
      hints.setIsPure(config.pure);
      hints.setTryRotate(true);
      hints.setTryHarder(true);
      hints.setTryDownscale(true);
      hints.setTryInvert(true);
      hints.setReturnErrors(true);

      // coordinate system in opencv is different
      auto const &image = result.image; // dip::filtering::flipX(result.image);
      auto const view = ZXing::ImageView{image.data, image.cols, image.rows, ZXing::ImageFormat::Lum, (int)image.step, 1};
      zresult = ZXing::ReadBarcode(view, hints);
    }
  };

  AztecDecoder::AztecDecoder(::utility::LoggerFactory &loggerFactory, unsigned int id, cv::Rect const &box, cv::Mat const &image, api::Config config)
      : logger(CREATE_LOGGER(loggerFactory)), internal(std::make_shared<Internal>(api::Result(id, box, image), std::move(config)))
  {
  }

  api::Level AztecDecoder::detect()
  {
    internal->detectionFinished = true;
    if (!(internal->zresult.position().bottomRight() == ZXing::PointI{}))
    {
      internal->result.level = api::Level::Detected;
      LOG_DEBUG(logger) << "Detected";
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

    if (internal->zresult.contentType() != ZXing::ContentType::Binary)
    {
      LOG_WARN(logger) << "Non-binary content detected";
      return std::move(internal->result);
    }

    internal->result.level = api::Level::Decoded;
    LOG_DEBUG(logger) << "Decoded";

    internal->result.payload = internal->zresult.bytes();
    return std::move(internal->result);
  }
}
