#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

conan install ${WORKSPACE_ROOT} \
    -of ${WORKSPACE_ROOT}/build/${BUILD_TYPE} \
    -s build_type=${BUILD_TYPE} \
    --build missing \
    -pr amd64-macos -pr apple-clang16 \
    ${@:2}

conan cache clean '*'
