#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"

${WORKSPACE_ROOT}/etc/conan-config.sh
${WORKSPACE_ROOT}/etc/conan-install.sh Debug
${WORKSPACE_ROOT}/etc/cmake-config.sh Debug
${WORKSPACE_ROOT}/etc/cmake-build.sh Debug $@

export PYTHONPATH=${WORKSPACE_ROOT}/build/Debug/bin
