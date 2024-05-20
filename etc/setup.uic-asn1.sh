#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"/../

rm -rfI ${WORKSPACE_ROOT}/etc/UIC-barcode
pushd ${WORKSPACE_ROOT}/etc
git clone https://github.com/UnionInternationalCheminsdeFer/UIC-barcode.git
popd

readonly ASN1C_COMMAND="asn1c -fcompound-names -fwide-types -gen-PER ../../../../../../etc/UIC-barcode/misc"

pushd ${WORKSPACE_ROOT}/source/lib/uic918/u_flex/v1.3/gen
eval $ASN1C_COMMAND/uicRailTicketData_v1.3.4.asn
rm converter-sample.c
pushd ../../v2.0/gen
eval $ASN1C_COMMAND/uicRailTicketData_v2.0.2.asn
rm converter-sample.c
pushd ../../v3.0/gen
eval $ASN1C_COMMAND/uicRailTicketData_v3.0.3.asn
rm converter-sample.c
popd && popd && popd
