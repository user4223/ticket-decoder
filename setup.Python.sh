#!/usr/bin/env bash

set -o errexit

# create a matching conan profile if it is not existing
# conan profile new ticket-decoder --force --detect
./etc/conan-install.sh Release -o:h with_analyzer=False
./etc/cmake-config.sh Release
cmake --build build/Release/ --config Release -t ticket_decoder -- $@

export PYTHONPATH=`pwd`/build/Release/bin
