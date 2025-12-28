# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-detector-detail)

add_subdirectory(common)

IF (WITH_SQUARE_DETECTOR)
    add_subdirectory(square)
ENDIF()

IF (WITH_CLASSIFIER_DETECTOR)
    add_subdirectory(classifier)
ENDIF()
