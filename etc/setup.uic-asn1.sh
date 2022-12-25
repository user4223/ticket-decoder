#!/bin/bash

rm -rf etc/UIC-barcode
pushd etc
git clone https://github.com/UnionInternationalCheminsdeFer/UIC-barcode.git
popd 

rm -f source/lib/uic918/gen/v1.3/*
pushd source/lib/uic918/gen/v1.3
asn1c -fcompound-names -fwide-types -gen-PER ../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.3.asn
rm converter-sample.c
popd
