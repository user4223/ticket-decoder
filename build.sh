#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"
readonly BUILD_TYPE=${1:-Release}

${WORKSPACE_ROOT}/etc/cmake-build.sh ${BUILD_TYPE} ${@:2}

export PYTHONPATH=${WORKSPACE_ROOT}/build/${BUILD_TYPE}/bin
