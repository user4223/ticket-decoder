// SPDX-FileCopyrightText: (C) 2026 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#if defined(_MSC_VER)
#include <stdint.h>
#include <BaseTsd.h>

#define ASSUMESTDTYPES
#define __attribute__(attributes)
typedef SSIZE_T ssize_t;
#endif
