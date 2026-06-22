#!/usr/bin/env bash
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../

# create tempory storage location
#
mkdir -p ${WORKSPACE_ROOT}/images/temp/

# download sample ticket files first
#
wget https://assets.static-bahn.de/dam/jcr:c362849f-210d-4dbe-bb18-34141b5ba274/mdb_320951_muster-tickets_nach_uic_918-3_2.zip \
    -O ${WORKSPACE_ROOT}/images/temp/uic918-3.zip
wget https://assets.static-bahn.de/dam/jcr:ec74454d-557b-438f-8ed9-689abcc276f5/Muster%20918-9.zip \
    -O ${WORKSPACE_ROOT}/images/temp/uic918-9.zip
wget https://www.kcd-nrw.de/fileadmin/user_upload/2023-05-05_Testtickets.zip \
    -O ${WORKSPACE_ROOT}/images/temp/vdv-ka.zip

# only when download succeeded, remove folders containing legacy files
#
rm -rfI ${WORKSPACE_ROOT}/images/Muster-UIC918-3
rm -rfI ${WORKSPACE_ROOT}/images/Muster-UIC918-9
rm -rfI ${WORKSPACE_ROOT}/images/Testtickets

# on macos, default unzip app is not able to handle german character encoding (-I) 4 filenames properly, so we use python
#unzip -I ISO-8859-1 -j ${WORKSPACE_ROOT}/images/temp/uic918-3.zip -d ${WORKSPACE_ROOT}/images/Muster-UIC918-3/
#unzip -I ISO-8859-1 -j ${WORKSPACE_ROOT}/images/temp/uic918-9.zip -d ${WORKSPACE_ROOT}/images/Muster-UIC918-9/

# unzip and move zip file content into subfolder in images
#
python3 -m zipfile -e ${WORKSPACE_ROOT}/images/temp/uic918-3.zip ${WORKSPACE_ROOT}/images/
mv "${WORKSPACE_ROOT}/images/Muster-Tickets nach UIC 918-3" ${WORKSPACE_ROOT}/images/Muster-UIC918-3/

python3 -m zipfile -e ${WORKSPACE_ROOT}/images/temp/uic918-9.zip ${WORKSPACE_ROOT}/images/
mv "${WORKSPACE_ROOT}/images/Muster 918-9" ${WORKSPACE_ROOT}/images/Muster-UIC918-9/

python3 -m zipfile -e ${WORKSPACE_ROOT}/images/temp/vdv-ka.zip ${WORKSPACE_ROOT}/images/

# remove temporary storage location
#
rm -rf ${WORKSPACE_ROOT}/images/temp
