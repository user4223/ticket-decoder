#include "../include/AztecDecoder.h"

std::unique_ptr<Decoder> AztecDecoder::create()
{
  return std::unique_ptr<Decoder>{new AztecDecoder()};
}

std::tuple<bool, std::string> AztecDecoder::detect(cv::Mat const &image)
{
  return {false, ""};
}