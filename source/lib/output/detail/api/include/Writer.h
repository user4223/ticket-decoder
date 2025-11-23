// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <opencv2/core.hpp>

#include <filesystem>
#include <vector>
#include <string>
#include <cstdint>

namespace output::detail
{
    class Writer
    {
    public:
        virtual ~Writer() = default;

        virtual std::filesystem::path write(cv::Mat const &image, std::string postfix = "") = 0;

        virtual std::filesystem::path write(std::vector<std::uint8_t> const &bytes, std::string postfix = "") = 0;

        virtual std::filesystem::path write(std::string const &json, std::string postfix = "") = 0;
    };
}
