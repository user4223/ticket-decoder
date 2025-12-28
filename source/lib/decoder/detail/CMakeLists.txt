# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-decoder-detail)

IF (WITH_BARCODE_DECODER)
    add_subdirectory(zxing)
ENDIF()
