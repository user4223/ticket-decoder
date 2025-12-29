# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-interpreter-api)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   easyloggingpp::easyloggingpp
   nlohmann_json::nlohmann_json
   ticket-decoder-interpreter-detail-common)

IF (WITH_UIC_INTERPRETER)
   target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-interpreter-detail-uic918)
ENDIF()

IF (WITH_VDV_INTERPRETER)
   target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-interpreter-detail-vdv)
ENDIF()

IF (WITH_SBB_INTERPRETER)
   target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-interpreter-detail-sbb)
ENDIF()

IF (WITH_SIGNATURE_VERIFIER)
   target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-interpreter-detail-verifier)
ENDIF()
