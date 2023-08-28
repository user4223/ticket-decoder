cmake_minimum_required(VERSION 3.15)
project(ticket-decoder)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json to make auto-complete working in vscode
cmake_policy(SET CMP0063 NEW) # To make the visibility setting below working
set(CMAKE_CXX_VISIBILITY_PRESET hidden) # Due to static linking, we get visibility warning otherwiese, since libs are using hidden und default here would lead to public

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

add_compile_options(-Wno-deprecated)

include(${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

add_subdirectory(source/lib)
add_subdirectory(source/app)
add_subdirectory(source/test)
