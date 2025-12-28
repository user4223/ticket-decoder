# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-detector-api)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   opencv::opencv_core_alias
   easyloggingpp::easyloggingpp
   ticket-decoder-detector-detail-common)

IF (WITH_SQUARE_DETECTOR)
    target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-detector-detail-square)
ENDIF()

IF (WITH_CLASSIFIER_DETECTOR)
    target_link_libraries(${PROJECT_NAME} PRIVATE ticket-decoder-detector-detail-classifier)
ENDIF()
