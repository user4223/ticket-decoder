// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>

namespace test::support
{
    class TempPath
    {
        std::filesystem::path path;

    public:
        TempPath(bool changeCurrentDirectory = true);
        ~TempPath();

        std::filesystem::path get() const { return path; }
    };
}
