// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "../include/Result.h"

namespace decoder::api
{

  Result::Result(unsigned int id, cv::Rect const &b, cv::Mat const &i)
      : id(id), box(b), image(i)
  {
  }

  bool Result::isDecoded() const
  {
    return level == decoder::api::Level::Decoded && !payload.empty();
  }
}
