#!/bin/bash

# create a matching conan profile if it is not existing
# conan profile new ticket-decoder --force --detect
conan install . -if build/Debug -pr ticket-decoder -s build_type=Debug --build missing
cmake -S . -B build/Debug/ -DCMAKE_BUILD_TYPE=Debug
cmake --build build/Debug/ --config Debug -- $@
