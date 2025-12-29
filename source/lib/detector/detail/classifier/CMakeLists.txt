# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket-decoder-detector-detail-classifier)

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)
file(GLOB PROJECT_INCLUDES "include/*.h")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE} ${PROJECT_INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE
   opencv::opencv_core_alias
   opencv::opencv_imgproc_alias
   opencv::opencv_objdetect_alias
   easyloggingpp::easyloggingpp
   ticket-decoder-detector-detail-common)

add_custom_target(${PROJECT_NAME}.Copy ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/etc $<TARGET_FILE_DIR:${PROJECT_NAME}>/etc/detector/classifier
   DEPENDS ${PROJECT_NAME}
   COMMENT "Copying classifier detector configuration files")
