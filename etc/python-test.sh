#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

PYTHONPATH=${WORKSPACE_ROOT}/build/${BUILD_TYPE}/bin \
    python3 -m unittest discover \
        -s ${WORKSPACE_ROOT}/source/test/python/
