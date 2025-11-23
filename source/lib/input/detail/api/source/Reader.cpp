// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Reader.h"

#include "lib/input/detail/image/include/ImageReader.h"
#ifdef WITH_PDF_INPUT
#include "lib/input/detail/pdf/include/PdfReader.h"
#endif

#include "lib/utility/include/FileSystem.h"

namespace input::detail
{
    std::vector<std::shared_ptr<Reader>> Reader::createAll(infrastructure::Context &context, api::LoadOptions options)
    {
        return {
            std::shared_ptr<Reader>(new ImageReader(context, options)),
#ifdef WITH_PDF_INPUT
            std::shared_ptr<Reader>(new PdfReader(context, options)),
#endif
        };
    }

    void Reader::validate(std::filesystem::path path, std::vector<std::string> allowedLowerCaseExtensions)
    {
        if (!std::filesystem::exists(path))
        {
            throw std::runtime_error("Requested input file does not exist: " + path.string());
        }
        if (!std::filesystem::is_regular_file(path))
        {
            throw std::runtime_error("Requested input file is not a regular file: " + path.string());
        }

        auto const extension = utility::normalizeExtension(path);
        if (std::find(allowedLowerCaseExtensions.cbegin(), allowedLowerCaseExtensions.cend(), extension) == allowedLowerCaseExtensions.cend())
        {
            throw std::runtime_error("Requested input file has unexpected extension: " + path.string());
        }
    }
}
