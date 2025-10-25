#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../../
readonly BUILD_TYPE=${1:-Release}

# Dependencies like eaasyloggingcpp are still compatible with quite old cmake versions
# like v2.8 but the support for this has been remove in cmake v4. So to make them
# compilable, we force compatibility mode of cmake v3.5 but this might fail at any point.
export CMAKE_POLICY_VERSION_MINIMUM=3.5

conan create ${WORKSPACE_ROOT}/etc/poppler \
    --build=missing \
    -s build_type=${BUILD_TYPE} \
    ${@:2}

conan cache clean '*'
