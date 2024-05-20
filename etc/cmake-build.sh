#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

cmake --build ${WORKSPACE_ROOT}/build/${BUILD_TYPE}/ \
    --config ${BUILD_TYPE} \
    ${@:2}
