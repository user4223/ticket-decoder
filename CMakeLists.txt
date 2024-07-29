cmake_minimum_required(VERSION 3.19)
project(ticket-decoder)

set(CONAN_DISABLE_CHECK_COMPILER ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json to make auto-complete working in vscode
cmake_policy(SET CMP0063 NEW) # To make the visibility setting below working
set(CMAKE_CXX_VISIBILITY_PRESET hidden) # Due to static linking, we get visibility warning otherwiese, since libs are using hidden und default here would lead to public
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-stdlib=libc++>)
endif()

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++20>)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)
find_package(ZXing REQUIRED)
find_package(easyloggingpp REQUIRED)
find_package(poppler REQUIRED)
find_package(botan REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(pugixml REQUIRED)
find_package(tclap REQUIRED)

IF (TARGET opencv::opencv_highgui_alias AND TARGET opencv::opencv_videoio_alias)
  SET(WITH_TICKET_ANALYZER TRUE)
ELSE()
  SET(WITH_TICKET_ANALYZER FALSE)
ENDIF()

add_subdirectory(source/lib)
add_subdirectory(source/app)
add_subdirectory(source/python)
add_subdirectory(source/test)
