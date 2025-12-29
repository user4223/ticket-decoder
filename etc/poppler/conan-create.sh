#!/usr/bin/env bash
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

set -o errexit

[[ "$@" =~ 'with_pdf_input=False' ]] && { echo "PDF input disabled, skipping local build of libpoppler dependency"; exit 0; }

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

# Remove temporary stuff like source and build folders 2 keep cache folder as small as possible.
# This does NOT remove the created binaries.
conan cache clean '*'
