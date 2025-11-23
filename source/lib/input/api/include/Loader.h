// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "LoadOptions.h"
#include "LoadResult.h"

#include "lib/input/api/include/InputElement.h"

#include "lib/infrastructure/include/ContextFwd.h"
#include "lib/utility/include/Logger.h"

#include <filesystem>
#include <vector>
#include <map>
#include <functional>
#include <future>

namespace input::detail
{
    class Reader;
};

namespace input::api
{
    class Loader
    {
    public:
        using ReaderMap = std::map<std::string, std::shared_ptr<detail::Reader>>;

    private:
        ::utility::Logger logger;
        ReaderMap const readers;

    public:
        Loader(infrastructure::Context &context, std::vector<std::shared_ptr<detail::Reader>> readers);

        Loader(infrastructure::Context &context, LoadOptions options = {});

        LoadResult load(std::filesystem::path path) const;

        size_t load(std::filesystem::path path, std::function<void(InputElement &&)> handler) const;

        LoadResult loadAsync(std::filesystem::path path) const;

        std::future<size_t> loadAsync(std::filesystem::path path, std::function<void(InputElement &&)> handler) const;
    };
}
