// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/infrastructure/include/Context.h"
#include "lib/utility/include/LoggingFwd.h"

#include <opencv2/core.hpp>

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <filesystem>

namespace interpreter::api
{
  class SignatureVerifier;
}

namespace test::support
{
  class TestSupport;

  TestSupport &get();

  class TestSupport
  {
    std::filesystem::path const executableFolderPath;
    std::unique_ptr<infrastructure::Context> context;
    std::unique_ptr<interpreter::api::SignatureVerifier> signatureVerifier;
    std::map<std::size_t, std::vector<std::uint8_t>> dummyImages;

  public:
    TestSupport(int argc, char **argv);
    TestSupport(std::filesystem::path executablePath);
    TestSupport(TestSupport &) = delete;
    TestSupport(TestSupport &&) = delete;
    TestSupport &operator=(TestSupport &) = delete;
    TestSupport &operator=(TestSupport &&) = delete;

    ~TestSupport();

    struct ContextDisabler;
    std::shared_ptr<ContextDisabler> disableContextTemporarily();

    infrastructure::Context &getContext();

    ::utility::LoggerFactory &getLoggerFactory();

    std::filesystem::path getExecutableFolderPath();

    std::filesystem::path getIOPath();

    interpreter::api::SignatureVerifier &getSignatureChecker();

    std::vector<std::uint8_t> getInterpreterData(std::string fileName);

    cv::Mat getDecoderImage(std::string fileName);

    cv::Mat getDummyImage(std::size_t widthHeight = 1);
  };
}
