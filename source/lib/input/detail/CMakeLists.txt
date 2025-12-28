# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-input-detail)

add_subdirectory(api)
add_subdirectory(image)
IF (WITH_PDF_INPUT)
    add_subdirectory(pdf)
ENDIF()
