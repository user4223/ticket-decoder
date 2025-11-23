// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

namespace utility
{
  unsigned int safeDecrement(unsigned int &value, unsigned int min);

  unsigned int safeIncrement(unsigned int &value, unsigned int max);

  unsigned int safeIncrement(unsigned int &value, unsigned int increment, unsigned int max);

  unsigned int rotate(unsigned int &value, unsigned int max);

  int rotate(int &value, int increment, int max);

  int rotate(int &value, int increment, int min, int max);
}
