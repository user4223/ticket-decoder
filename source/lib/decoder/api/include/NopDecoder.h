// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Decoder.h"

namespace decoder::api
{
  class NopDecoder : public api::Decoder
  {
  public:
    virtual api::Result decode(detector::api::Descriptor const &descriptor);

    virtual api::Result decode(unsigned int id, cv::Rect const &box, cv::Mat const &image);
  };
}
