#!/usr/bin/env bash
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"
readonly BUILD_TYPE=${1:-Release}

${WORKSPACE_ROOT}/etc/conan-config.sh
${WORKSPACE_ROOT}/etc/conan-install.sh ${BUILD_TYPE} \
    -o "&:with_analyzer=True" \
    -o "&:with_python_module=True" \
    -o "&:with_square_detector=True" \
    -o "&:with_classifier_detector=True" \
    -o "&:with_barcode_decoder=True" \
    -o "&:with_pdf_input=True" \
    -o "&:with_signature_verifier=True" \
    -o "&:with_uic_interpreter=True" \
    -o "&:with_sbb_interpreter=True"

${WORKSPACE_ROOT}/etc/cmake-config.sh ${BUILD_TYPE}
${WORKSPACE_ROOT}/build.sh ${BUILD_TYPE} ${@:2}

export PYTHONPATH=${WORKSPACE_ROOT}/build/${BUILD_TYPE}/bin
