#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/..

# curl --output ${WORKSPACE_ROOT}/cert/UIC_PublicKeys.xml \
#     --show-error --fail 'https://railpublickey.uic.org/download.php'

wget -nv -O ${WORKSPACE_ROOT}/cert/UIC_PublicKeys.xml \
    'https://railpublickey.uic.org/download.php'
