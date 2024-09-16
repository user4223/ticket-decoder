#include "../include/AztecDecoder.h"

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/Logging.h"

#include "ZXing/ReadBarcode.h"

#include "lib/dip/filtering/include/Transform.h"

namespace barcode::detail
{

  ZXing::ReaderOptions createOptions(::utility::DebugController &debugController, api::DecoderOptions decoderOptions)
  {
    ZXing::ReaderOptions options;
    options.setFormats(ZXing::BarcodeFormat::Aztec);
    options.setCharacterSet(ZXing::CharacterSet::BINARY);
    options.setBinarizer(debugController.getAs<bool>("aztecDecoder.binarize")
                             ? ZXing::Binarizer::LocalAverage
                             : ZXing::Binarizer::BoolCast);
    options.setIsPure(debugController.getAs<bool>("aztecDecoder.pure"));
    options.setTryRotate(debugController.getAs<bool>("aztecDecoder.tryRotate"));
    options.setTryHarder(debugController.getAs<bool>("aztecDecoder.tryHarder"));
    options.setTryDownscale(debugController.getAs<bool>("aztecDecoder.tryDownscale"));
    options.setTryInvert(debugController.getAs<bool>("aztecDecoder.tryInvert"));
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
        LOG_DEBUG(logger) << "No aztec-code detected: " << zresult.error().msg();
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

  AztecDecoder::AztecDecoder(infrastructure::Context &context, api::DecoderOptions defaultOptions)
      : debugController(context.getDebugController()
                            .define("aztecDecoder.binarize", {defaultOptions.binarize, "ad.binarize"})
                            .define("aztecDecoder.pure", {defaultOptions.pure, "ad.pure"})
                            .define("aztecDecoder.tryRotate", {true, "ad.rotate"})
                            .define("aztecDecoder.tryHarder", {true, "ad.harder"})
                            .define("aztecDecoder.tryDownscale", {true, "ad.downscale"})
                            .define("aztecDecoder.tryInvert", {true, "ad.invert"})),
        internal(std::make_shared<Internal>(CREATE_LOGGER(context.getLoggerFactory()), debugController, std::move(defaultOptions)))
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
