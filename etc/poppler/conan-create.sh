#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../../
readonly BUILD_TYPE=${1:-Release}

conan create --build=missing \
    -pr ticket-decoder -pr:b ticket-decoder \
    -s:b "build_type=${BUILD_TYPE}" \
    -o:b "*:CMAKE_POLICY_VERSION_MINIMUM=3.5" \
    ${WORKSPACE_ROOT}/etc/poppler

conan cache clean '*'
