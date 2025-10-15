#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../../
readonly BUILD_TYPE=${1:-Release}

conan create ${WORKSPACE_ROOT}/etc/poppler \
    --build=missing \
    -pr:b ticket-decoder \
    -s:b "build_type=${BUILD_TYPE}" \
    -o:b "*:CMAKE_POLICY_VERSION_MINIMUM=3.5"

conan cache clean '*'
