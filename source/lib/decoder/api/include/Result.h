// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Level.h"

#include <opencv2/core.hpp>

#include <vector>
#include <cstdint>

namespace decoder::api
{
  struct Result
  {
    unsigned int id;
    cv::Rect box;
    cv::Mat image;
    Level level = Level::Unknown;
    std::vector<std::uint8_t> payload;

    Result() = default;
    Result(unsigned int id, cv::Rect const &box, cv::Mat const &image);

    Result(Result &&) = default;
    Result &operator=(Result &&) = default;

    Result(Result const &) = delete;
    Result &operator=(Result const &) = delete;

    bool isDecoded() const;
  };
}
