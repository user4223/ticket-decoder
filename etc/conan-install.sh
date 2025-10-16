#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

# We're building poppler with cpp bindings with minimal dependencies for our own since
# the official recipe is quite old and there is a lot of complexity due to lots of wrappers,
# options and stuff like this. We avoid this by using an own stripped minimal recipe.
${WORKSPACE_ROOT}/etc/poppler/conan-create.sh ${BUILD_TYPE} ${@:2}

export CMAKE_POLICY_VERSION_MINIMUM=3.5

conan install ${WORKSPACE_ROOT} \
    --build missing \
    -of ${WORKSPACE_ROOT}/build/${BUILD_TYPE} \
    -s:b "build_type=${BUILD_TYPE}" \
    ${@:2}

conan cache clean '*'
