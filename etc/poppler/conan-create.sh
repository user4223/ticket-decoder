#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../../
readonly BUILD_TYPE=${1:-Release}

export CMAKE_POLICY_VERSION_MINIMUM=3.5

conan create ${WORKSPACE_ROOT}/etc/poppler \
    --build=missing \
    -s:b "build_type=${BUILD_TYPE}" \
    ${@:2}

conan cache clean '*'
