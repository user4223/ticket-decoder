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

    Internal(api::Result &&r, bool p)
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
      // TODO: Make it a parameter and swap it depending on source
      //   - BoolCast is working very well 4 existing images
      //   - LocalAverage is working better 4 camera
      hints.setBinarizer(ZXing::Binarizer::LocalAverage);
      hints.setCharacterSet(ZXing::CharacterSet::BINARY);
      hints.setIsPure(p);
      hints.setTryRotate(true);
      hints.setTryHarder(true);

      // coordinate system in opencv is different
      auto const &image = result.image; // dip::filtering::flipX(result.image);
      auto const view = ZXing::ImageView{image.data, image.cols, image.rows, ZXing::ImageFormat::Lum, (int)image.step, 1};
      zresult = ZXing::ReadBarcode(view, hints);
    }
  };

  AztecDecoder::AztecDecoder(::utility::LoggerFactory &loggerFactory, unsigned int id, cv::Rect const &box, cv::Mat const &image, bool const pure)
      : logger(CREATE_LOGGER(loggerFactory)), internal(std::make_shared<Internal>(api::Result(id, box, image), pure))
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

    internal->result.level = api::Level::Decoded;
    LOG_DEBUG(logger) << "Decoded";

    internal->result.payload = internal->zresult.bytes();
    return std::move(internal->result);
  }
}
