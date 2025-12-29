#!/usr/bin/env bash
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../
readonly BUILD_TYPE=${1:-Release}

PYTHONPATH=${WORKSPACE_ROOT}/build/${BUILD_TYPE}/bin \
    python3 -m unittest discover \
        -s ${WORKSPACE_ROOT}/source/test/python/
