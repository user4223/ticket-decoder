#!/bin/bash

conan install . -of build/Debug -pr:h ticket-decoder -pr:b ticket-decoder -s build_type=Debug --build missing
cmake -S . -B build/Debug/ --preset conan-debug
cmake --build build/Debug/ --preset conan-debug -- $@
