
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/dip/filtering/include/PreProcessor.h"
#include "test/support/include/Loader.h"

namespace dip::filtering
{
  static auto loggerFactory = ::utility::LoggerFactory::createLazy(true);

  TEST(PreProcessor, splitStringToPair1)
  {
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair(""));
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("blubber"));
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("00"));
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("11"));
  }

  TEST(PreProcessor, splitStringToPair2)
  {
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("20"));
    EXPECT_EQ(std::make_pair(2u, 1u), splitStringToPair("21"));
    EXPECT_EQ(std::make_pair(2u, 2u), splitStringToPair("22"));
  }

  TEST(PreProcessor, splitStringToPair4)
  {
    EXPECT_EQ(std::make_pair(1u, 1u), splitStringToPair("40"));
    EXPECT_EQ(std::make_pair(4u, 1u), splitStringToPair("41"));
    EXPECT_EQ(std::make_pair(4u, 2u), splitStringToPair("42"));
    EXPECT_EQ(std::make_pair(4u, 3u), splitStringToPair("43"));
    EXPECT_EQ(std::make_pair(4u, 4u), splitStringToPair("44"));
  }

  TEST(PreProcessor, splitPairToMap1)
  {
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({0, 0}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({1, 1}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({3, 2}));
  }

  TEST(PreProcessor, splitPairToMap2)
  {
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({2, 0}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 1u}, {4u, 0u}}), splitPairToMap({2, 1}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 2u}, {4u, 0u}}), splitPairToMap({2, 2}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 2u}, {4u, 0u}}), splitPairToMap({2, 3}));
  }

  TEST(PreProcessor, splitPairToMap4)
  {
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 0u}}), splitPairToMap({4, 0}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 1u}}), splitPairToMap({4, 1}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 2u}}), splitPairToMap({4, 2}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 3u}}), splitPairToMap({4, 3}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 4u}}), splitPairToMap({4, 4}));
    EXPECT_EQ((std::map<unsigned int, unsigned int>{{2u, 0u}, {4u, 4u}}), splitPairToMap({4, 5}));
  }

  TEST(PreProcessor, split4)
  {
    auto preProcessor = PreProcessor::create(loggerFactory);
    auto data = std::vector<std::uint8_t>{1, 2, 3, 4};
    auto const input = cv::Mat{2, 2, CV_8UC1, data.data(), 2};
    EXPECT_EQ(1, input.at<std::uint8_t>(0, 0));
    EXPECT_EQ(2, input.at<std::uint8_t>(0, 1));
    EXPECT_EQ(3, input.at<std::uint8_t>(1, 0));
    EXPECT_EQ(4, input.at<std::uint8_t>(1, 1));
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(3, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(4, output.at<std::uint8_t>(1, 1));
    }
    preProcessor.toggleSplit4();
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.rows);
      EXPECT_EQ(1, output.cols);
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
    }
    preProcessor.toggleSplit4();
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.rows);
      EXPECT_EQ(1, output.cols);
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 0));
    }
    preProcessor.toggleSplit4();
    // ...
    preProcessor.toggleSplit4();
    preProcessor.toggleSplit4();
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(3, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(4, output.at<std::uint8_t>(1, 1));
    }
  }

  TEST(PreProcessor, split2)
  {
    auto preProcessor = PreProcessor::create(loggerFactory);
    auto data = std::vector<std::uint8_t>{1, 2, 3, 4};
    auto const input = cv::Mat{2, 2, CV_8UC1, data.data(), 2};
    EXPECT_EQ(1, input.at<std::uint8_t>(0, 0));
    EXPECT_EQ(2, input.at<std::uint8_t>(0, 1));
    EXPECT_EQ(3, input.at<std::uint8_t>(1, 0));
    EXPECT_EQ(4, input.at<std::uint8_t>(1, 1));
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(3, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(4, output.at<std::uint8_t>(1, 1));
    }
    preProcessor.toggleSplit2();
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(2, output.rows);
      EXPECT_EQ(1, output.cols);
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
    }
    preProcessor.toggleSplit2();
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(2, output.rows);
      EXPECT_EQ(1, output.cols);
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 0));
    }
    preProcessor.toggleSplit2();
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(3, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(4, output.at<std::uint8_t>(1, 1));
    }
  }

  TEST(PreProcessor, enable)
  {
    auto preProcessor = PreProcessor::create(loggerFactory);
    auto data = std::vector<std::uint8_t>{1, 2, 3, 4};
    auto const input = cv::Mat{2, 2, CV_8UC1, data.data(), 2};
    preProcessor.toggleSplit4();
    preProcessor.toggleSplit4();
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.rows);
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 0));
    }
    preProcessor.enable(false);
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(2, output.rows);
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
    }
    preProcessor.enable(true);
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.rows);
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 0));
    }
  }

  TEST(PreProcessor, rotate)
  {
    auto preProcessor = PreProcessor::create(loggerFactory);
    auto data = std::vector<std::uint8_t>{1, 2, 3, 4};
    auto const input = cv::Mat{2, 2, CV_8UC1, data.data(), 2};
    for (int i = 0; i < 90; ++i)
    {
      preProcessor.rotateCCW();
    }
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 0)); // I have no idea why the result is in this order, but the images on screen are looking fine
      EXPECT_EQ(4, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(1, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(3, output.at<std::uint8_t>(1, 1));
    }
    for (int i = 0; i < 90; ++i)
    {
      preProcessor.rotateCW();
    }
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(3, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(4, output.at<std::uint8_t>(1, 1));
    }
  }

  TEST(PreProcessor, scale)
  {
    auto preProcessor = PreProcessor::create(loggerFactory);
    auto data = std::vector<std::uint8_t>{1, 2, 3, 4};
    auto const input = cv::Mat{2, 2, CV_8UC1, data.data(), 2};
    for (int i = 0; i < 100; ++i)
    {
      preProcessor.scaleUp();
    }
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(4, output.rows);
      EXPECT_EQ(4, output.cols);
    }
    for (int i = 0; i < 100; ++i)
    {
      preProcessor.scaleDown();
    }
    {
      auto output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(2, output.rows);
      EXPECT_EQ(2, output.cols);
    }
  }

  TEST(PreProcessor, flip)
  {
    auto preProcessor = PreProcessor::create(loggerFactory);
    auto data = std::vector<std::uint8_t>{1, 2, 3, 4};
    auto const input = cv::Mat{2, 2, CV_8UC1, data.data(), 2};
    {
      auto const output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(3, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(4, output.at<std::uint8_t>(1, 1));
    }
    preProcessor.toggleFlipping(); // X
    {
      auto const output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(3, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(4, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(1, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(2, output.at<std::uint8_t>(1, 1));
    }
    preProcessor.toggleFlipping(); // Y
    {
      auto const output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(2, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(1, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(4, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(3, output.at<std::uint8_t>(1, 1));
    }
    preProcessor.toggleFlipping(); // XY
    {
      auto const output = preProcessor.get(io::api::InputElement::fromCamera(input.clone())).getImage();
      EXPECT_EQ(4, output.at<std::uint8_t>(0, 0));
      EXPECT_EQ(3, output.at<std::uint8_t>(0, 1));
      EXPECT_EQ(2, output.at<std::uint8_t>(1, 0));
      EXPECT_EQ(1, output.at<std::uint8_t>(1, 1));
    }
  }
}
