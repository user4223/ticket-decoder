#!/usr/bin/env bash

set -o errexit

# create a matching conan profile if it is not existing
# conan profile new ticket-decoder --force --detect
./etc/conan-install.sh Debug
./etc/cmake-config.sh Debug
cmake --build build/Debug/ --config Debug -- $@

export PYTHONPATH=`pwd`/build/Debug/bin
