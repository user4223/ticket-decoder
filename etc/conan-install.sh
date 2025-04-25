#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

# We're building poppler with cpp bindings with minimal dependencies for our own since
# the official recipe is quite old and there is a lot of complexity due to lots of wrappers,
# options and stuff like this. We avoid this by using an own stripped minimal recipe.
${WORKSPACE_ROOT}/etc/poppler/conan-create.sh ${BUILD_TYPE}

conan install ${WORKSPACE_ROOT} \
    -if ${WORKSPACE_ROOT}/build/${BUILD_TYPE} \
    -pr ticket-decoder -pr:b ticket-decoder \
    -s build_type=${BUILD_TYPE} \
    --build missing \
    -e CMAKE_POLICY_VERSION_MINIMUM=3.5 \
    ${@:2}

conan remove '*' --build --src --force
