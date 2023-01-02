#!/bin/bash

#rm -f images/*
pushd images
wget https://assets.static-bahn.de/dam/jcr:c362849f-210d-4dbe-bb18-34141b5ba274/mdb_320951_muster-tickets_nach_uic_918-3_2.zip -O uic918-3.zip
wget https://assets.static-bahn.de/dam/jcr:3c7a020a-7632-4f23-8716-6ebfc9f93ccb/Muster%20918-9.zip -O uic918-9.zip
popd
