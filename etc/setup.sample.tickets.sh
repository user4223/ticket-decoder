#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../

pushd ${WORKSPACE_ROOT}/images
wget https://assets.static-bahn.de/dam/jcr:c362849f-210d-4dbe-bb18-34141b5ba274/mdb_320951_muster-tickets_nach_uic_918-3_2.zip -O uic918-3.zip
wget https://assets.static-bahn.de/dam/jcr:95540b93-5c38-4554-8f00-676214f4ba76/Muster%20918-9.zip -O uic918-9.zip
popd
