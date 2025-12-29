#!/usr/bin/env bash
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

# We're building poppler with cpp bindings with minimal dependencies for our own since
# the official recipe is quite old and there is a lot of complexity due to lots of wrappers,
# options and stuff like this. We avoid this by using an own stripped minimal recipe.
${WORKSPACE_ROOT}/etc/poppler/conan-create.sh ${BUILD_TYPE} ${@:2}

# Dependencies like eaasyloggingcpp are still compatible with quite old cmake versions
# like v2.8 but the support for this has been remove in cmake v4. So to make them
# compilable, we force compatibility mode of cmake v3.5 but this might fail at any point.
export CMAKE_POLICY_VERSION_MINIMUM=3.5

# Recipe for transitive dependency libxml2 does not handle conan-setting build_type properly
# so we have to define this env var to make this library building.
export CMAKE_BUILD_TYPE=${BUILD_TYPE}

conan install ${WORKSPACE_ROOT} \
    --build missing \
    -of ${WORKSPACE_ROOT}/build/${BUILD_TYPE} \
    -s:a build_type=${BUILD_TYPE} \
    -s:a compiler.cppstd=20 \
    ${@:2}

# Remove temporary stuff like source and build folders 2 keep cache folder as small as possible.
# This does NOT remove the created binaries.
conan cache clean '*'
