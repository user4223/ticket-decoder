# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-input-camera)

file(GLOB PROJECT_INCLUDES "include/*.h")

IF (WITH_TICKET_ANALYZER)

   ADD_LIBRARY(${PROJECT_NAME} STATIC "source/Camera.cpp" ${PROJECT_INCLUDES})
   target_link_libraries(${PROJECT_NAME} PRIVATE
      opencv::opencv_core_alias
      opencv::opencv_highgui_alias
      easyloggingpp::easyloggingpp)

ELSE()

   ADD_LIBRARY(${PROJECT_NAME} STATIC "source/Dummy.cpp" ${PROJECT_INCLUDES})
   target_link_libraries(${PROJECT_NAME} PRIVATE
      opencv::opencv_core_alias)

ENDIF()
