#include "../include/AztecDecoder.h"

#include "ZXing/ReadBarcode.h"

#include "lib/dip/filtering/include/Transform.h"

#include "lib/utility/include/Logging.h"

namespace barcode::detail
{

  ZXing::ReaderOptions createOptions(api::DecoderOptions decoderOptions)
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
    ZXing::ReaderOptions const defaultOptions;

    Internal(::utility::LoggerFactory &loggerFactory, ZXing::ReaderOptions options)
        : logger(CREATE_LOGGER(loggerFactory)),
          defaultOptions(options)
    {
    }

    api::Result decode(ZXing::ReaderOptions const &options, unsigned int id, cv::Rect const &box, cv::Mat const &image)
    {
      auto result = api::Result{id, box, image};

      if (result.image.empty())
      {
        return result;
      }

      auto const view = ZXing::ImageView{image.data, image.cols, image.rows, ZXing::ImageFormat::Lum, (int)image.step, 1};
      auto const zresult = ZXing::ReadBarcode(view, options);

      if (zresult.position().bottomRight() == ZXing::PointI{})
      {
        LOG_INFO(logger) << "No aztec-code detected";
        return result;
      }

      result.level = api::Level::Detected;
      LOG_DEBUG(logger) << "Detected";

      if (!zresult.isValid())
      {
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

  AztecDecoder::AztecDecoder(::utility::LoggerFactory &loggerFactory, api::DecoderOptions defaultOptions)
      : internal(std::make_shared<Internal>(loggerFactory, createOptions(std::move(defaultOptions))))
  {
  }

  api::Result AztecDecoder::decode(dip::detection::api::Descriptor const &descriptor)
  {
    return internal->decode(internal->defaultOptions, descriptor.id, descriptor.square, descriptor.image);
  }

  api::Result AztecDecoder::decode(api::DecoderOptions options, dip::detection::api::Descriptor const &descriptor)
  {
    return internal->decode(createOptions(options), descriptor.id, descriptor.square, descriptor.image);
  }

  api::Result AztecDecoder::decode(unsigned int id, cv::Rect const &box, cv::Mat const &image)
  {
    return internal->decode(internal->defaultOptions, id, box, image);
  }

  api::Result AztecDecoder::decode(api::DecoderOptions options, unsigned int id, cv::Rect const &box, cv::Mat const &image)
  {
    return internal->decode(createOptions(options), id, box, image);
  }
}
