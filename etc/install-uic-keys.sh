#!/usr/bin/env bash
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/..

# curl --output ${WORKSPACE_ROOT}/cert/UIC_PublicKeys.xml \
#     --show-error --fail 'https://railpublickey.uic.org/download.php'

mkdir -p ${WORKSPACE_ROOT}/cert
wget -nv -O ${WORKSPACE_ROOT}/cert/UIC_PublicKeys.xml \
    'https://railpublickey.uic.org/download.php'
