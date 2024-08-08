#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../

rm -rfI ${WORKSPACE_ROOT}/images/Muster-UIC918-3
rm -rfI ${WORKSPACE_ROOT}/images/Muster-UIC918-9
mkdir -p ${WORKSPACE_ROOT}/images/temp/

wget https://assets.static-bahn.de/dam/jcr:c362849f-210d-4dbe-bb18-34141b5ba274/mdb_320951_muster-tickets_nach_uic_918-3_2.zip \
    -O ${WORKSPACE_ROOT}/images/temp/uic918-3.zip
wget https://assets.static-bahn.de/dam/jcr:95540b93-5c38-4554-8f00-676214f4ba76/Muster%20918-9.zip \
    -O ${WORKSPACE_ROOT}/images/temp/uic918-9.zip

# on macos, default unzip app is not able to handle german character encoding (-I) 4 filenames properly, so we use python
#unzip -I ISO-8859-1 -j ${WORKSPACE_ROOT}/images/temp/uic918-3.zip -d ${WORKSPACE_ROOT}/images/Muster-UIC918-3/
#unzip -I ISO-8859-1 -j ${WORKSPACE_ROOT}/images/temp/uic918-9.zip -d ${WORKSPACE_ROOT}/images/Muster-UIC918-9/

python3 -m zipfile -e ${WORKSPACE_ROOT}/images/temp/uic918-3.zip ${WORKSPACE_ROOT}/images/
mv "${WORKSPACE_ROOT}/images/Muster-Tickets nach UIC 918-3" ${WORKSPACE_ROOT}/images/Muster-UIC918-3/
python3 -m zipfile -e ${WORKSPACE_ROOT}/images/temp/uic918-9.zip ${WORKSPACE_ROOT}/images/Muster-UIC918-9/

rm -rf ${WORKSPACE_ROOT}/images/temp
