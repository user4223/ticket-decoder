#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(dirname "$0")"

# create a matching conan profile if it is not existing
# conan profile new ticket-decoder --force --detect
${WORKSPACE_ROOT}/etc/conan-install.sh Release -o:h with_analyzer=False
${WORKSPACE_ROOT}/etc/cmake-config.sh Release
${WORKSPACE_ROOT}/etc/cmake-build.sh Release -t ticket_decoder $@

export PYTHONPATH=${WORKSPACE_ROOT}/build/Release/bin
