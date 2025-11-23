// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

namespace decoder::api
{
  struct DecoderOptions
  {
    bool pure = false;
    bool binarize = true;

    static DecoderOptions const DEFAULT;
  };
}
