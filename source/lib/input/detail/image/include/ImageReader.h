// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "lib/input/detail/api/include/Reader.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/infrastructure/include/Logger.h"

#include <opencv2/core.hpp>

#include <filesystem>

namespace input::detail
{
    class ImageReader : public Reader
    {
        infrastructure::Logger logger;
        api::LoadOptions options;

    public:
        ImageReader(infrastructure::Context &context, api::LoadOptions options);

        std::vector<std::string> supportedExtensions() const override;

        ReadResult read(std::filesystem::path path) const override;
    };
}
