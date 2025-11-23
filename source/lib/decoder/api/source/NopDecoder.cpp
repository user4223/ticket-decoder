// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/NopDecoder.h"

namespace decoder::api
{

  api::Result NopDecoder::decode(detector::api::Descriptor const &descriptor)
  {
    return api::Result();
  }

  api::Result NopDecoder::decode(unsigned int id, cv::Rect const &box, cv::Mat const &image)
  {
    return api::Result();
  }
}
