# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

PROJECT(ticket_decoder)

find_package(Python3 COMPONENTS Interpreter Development.Module)
find_package(Boost REQUIRED COMPONENTS python)

message("Python3_VERSION: " ${Python3_VERSION})

AUX_SOURCE_DIRECTORY("source" PROJECT_SOURCE)

# Important to have python package name as output folder inside bin to place __init__.py and shared u_flex libs beside
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})

ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SOURCE})

# Important to have no "lib" prefix!
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")
# Important to have no ".dylib" postfix on macos!
# TODO ".pyd" should be platform independent?
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SUFFIX ".so")

if(APPLE)                                              # macOS uses @loader_path for libraries and @executable_path for apps
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "@loader_path" BUILD_WITH_INSTALL_RPATH TRUE)
elseif(UNIX)                                      # Linux/Unix uses $ORIGIN
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "\${ORIGIN}" BUILD_WITH_INSTALL_RPATH TRUE)
endif()

target_link_options(${PROJECT_NAME} PUBLIC ${Python3_LINK_OPTIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    Boost::python
    Python3::Module
    easyloggingpp::easyloggingpp
    opencv::opencv_core_alias
    ticket-decoder-api
    ticket-decoder-dip)

IF (SKBUILD_PLATLIB_DIR)
    install(FILES ${PROJECT_SOURCE_DIR}/etc/__init__.py DESTINATION ${SKBUILD_PROJECT_NAME})
    install(TARGETS ticket-decoder-interpreter-detail-uic918-u_flex13 ticket-decoder-interpreter-detail-uic918-u_flex30 DESTINATION ${SKBUILD_PROJECT_NAME})
    install(TARGETS ${PROJECT_NAME} DESTINATION ${SKBUILD_PROJECT_NAME})
ELSE()
    file(INSTALL ${PROJECT_SOURCE_DIR}/etc/__init__.py DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:ticket-decoder-interpreter-detail-uic918-u_flex13>"
            "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:ticket-decoder-interpreter-detail-uic918-u_flex30>"
            "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        COMMENT "Copying shared u_flex libraries into python module output directory")
ENDIF()
