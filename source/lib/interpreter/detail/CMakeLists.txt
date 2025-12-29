# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-interpreter-detail)

add_subdirectory(common)

IF (WITH_UIC_INTERPRETER)
    add_subdirectory(uic918)
ENDIF()

IF (WITH_VDV_INTERPRETER)
    add_subdirectory(vdv)
ENDIF()

IF (WITH_SBB_INTERPRETER)
    add_subdirectory(sbb)
ENDIF()

IF (WITH_SIGNATURE_VERIFIER)
    add_subdirectory(verifier)
ENDIF()
