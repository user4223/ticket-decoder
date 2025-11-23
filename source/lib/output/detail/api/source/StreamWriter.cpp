// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/StreamWriter.h"

#include "lib/utility/include/Base64.h"

namespace output::detail
{
    struct StreamWriter::Internal
    {
        std::ostream &stream;
        std::string annotation;

        Internal(std::ostream &s, std::string a) : stream(s), annotation(std::move(a)) {}
    };

    StreamWriter::StreamWriter(std::ostream &stream, std::string annotation)
        : internal(std::make_shared<Internal>(stream, std::move(annotation)))
    {
    }

    std::filesystem::path StreamWriter::write(cv::Mat const &image, std::string postfix)
    {
        throw std::runtime_error("Cannot dump image to ostream");
    }

    std::filesystem::path StreamWriter::write(std::vector<std::uint8_t> const &bytes, std::string postfix)
    {
        return write(::utility::base64::encode(bytes), postfix);
    }

    std::filesystem::path StreamWriter::write(std::string const &json, std::string postfix)
    {
        internal->stream << json << std::endl;
        return internal->annotation;
    }
}
