#include "../include/AztecDecoder.h"

#include "ZXing/ReadBarcode.h"

#include "lib/dip/filtering/include/Transform.h"

#include "lib/utility/include/Logging.h"

namespace barcode::detail
{

  ZXing::ReaderOptions createOptions(::utility::DebugController &debugController, api::DecoderOptions decoderOptions)
  {
    ZXing::ReaderOptions options;
    options.setFormats(ZXing::BarcodeFormat::Aztec);
    options.setBinarizer(decoderOptions.binarize ? ZXing::Binarizer::LocalAverage : ZXing::Binarizer::BoolCast);
    options.setCharacterSet(ZXing::CharacterSet::BINARY);
    options.setIsPure(decoderOptions.pure);
    // TODO Make this parameters as well
    options.setTryRotate(true);
    options.setTryHarder(true);
    options.setTryDownscale(true);
    options.setTryInvert(true);
    options.setReturnErrors(true);
    return options;
  }

  struct AztecDecoder::Internal
  {
    ::utility::Logger logger;
    ::utility::DebugController &debugController;
    api::DecoderOptions defaultOptions;
    ZXing::ReaderOptions readerOptions;

    Internal(::utility::Logger l, ::utility::DebugController &dc, api::DecoderOptions o)
        : logger(std::move(l)),
          debugController(dc),
          defaultOptions(std::move(o)),
          readerOptions(createOptions(debugController, defaultOptions))
    {
    }

    ZXing::ReaderOptions const &getOptions()
    {
      if (debugController.touched())
      {
        readerOptions = createOptions(debugController, defaultOptions);
      }
      return readerOptions;
    }

    api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image)
    {
      auto result = api::Result{id, box, image};

      if (result.image.empty())
      {
        return result;
      }

      auto const view = ZXing::ImageView{image.data, image.cols, image.rows, ZXing::ImageFormat::Lum, (int)image.step, 1};
      auto const zresult = ZXing::ReadBarcode(view, getOptions());

      if (zresult.position().bottomRight() == ZXing::PointI{})
      {
        LOG_INFO(logger) << "No aztec-code detected: " << zresult.error().msg();
        return result;
      }

      result.level = api::Level::Detected;
      LOG_DEBUG(logger) << "Detected";

      if (!zresult.isValid())
      {
        LOG_INFO(logger) << "Aztec-code not valid: " << zresult.error().msg();
        return result;
      }

      if (zresult.contentType() != ZXing::ContentType::Binary)
      {
        LOG_WARN(logger) << "Non-binary content detected";
        return result;
      }

      result.level = api::Level::Decoded;
      LOG_DEBUG(logger) << "Decoded";

      result.payload = zresult.bytes();
      return result;
    }
  };

  AztecDecoder::AztecDecoder(::utility::LoggerFactory &loggerFactory, ::utility::DebugController &dctrl, api::DecoderOptions defaultOptions)
      : debugController(dctrl
                            .define("aztecDecoder.binarize", {defaultOptions.binarize, "ad.binarize"})
                            .define("aztecDecoder.pure", {defaultOptions.pure, "ad.pure"})),
        internal(std::make_shared<Internal>(CREATE_LOGGER(loggerFactory), debugController, std::move(defaultOptions)))
  {
  }

  api::Result AztecDecoder::decode(dip::detection::api::Descriptor const &descriptor)
  {
    return internal->decode(descriptor.id, descriptor.square, descriptor.image);
  }

  api::Result AztecDecoder::decode(unsigned int id, cv::Rect const &box, cv::Mat const &image)
  {
    return internal->decode(id, box, image);
  }
}
