#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../../
readonly BUILD_TYPE=${1:-Release}

conan create --build=missing \
    -pr ticket-decoder -pr:b ticket-decoder \
    -s build_type=${BUILD_TYPE} \
    -e CMAKE_POLICY_VERSION_MINIMUM=3.5 \
    ${WORKSPACE_ROOT}/etc/poppler

conan remove 'poppler-cpp' --build --src --force
