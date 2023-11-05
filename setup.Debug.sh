#!/bin/bash

# create a matching conan profile if it is not existing
# conan profile new ticket-decoder --force --detect
conan install . -if build/Debug -pr ticket-decoder -pr:b ticket-decoder -s build_type=Debug --build missing
# when presets are working for sure due to cmake >= 3.23, use: --preset debug
cmake -S . -B build/Debug/ -DCMAKE_TOOLCHAIN_FILE=build/Debug/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build build/Debug/ --config Debug -- $@
