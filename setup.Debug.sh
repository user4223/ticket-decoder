#!/bin/bash

conan install . -if build/Debug -s build_type=Debug --build missing
cmake -S . -B build/Debug/ -DCMAKE_BUILD_TYPE=Debug
cmake --build build/Debug/ --config Debug -- $@
