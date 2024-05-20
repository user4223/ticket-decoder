#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -e $(dirname "$0"))"

# create a matching conan profile if it is not existing
# conan profile new ticket-decoder --force --detect
${WORKSPACE_ROOT}/etc/conan-install.sh Debug
${WORKSPACE_ROOT}/etc/cmake-config.sh Debug
${WORKSPACE_ROOT}/etc/cmake-build.sh Debug $@

export PYTHONPATH=${WORKSPACE_ROOT}/build/Debug/bin
