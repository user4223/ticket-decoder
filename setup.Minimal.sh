#!/usr/bin/env bash

set -o errexit

readonly WORKSPACE_ROOT="$(readlink -f $(dirname "$0"))"
readonly BUILD_TYPE=${1:-Release}

${WORKSPACE_ROOT}/etc/conan-config.sh
${WORKSPACE_ROOT}/etc/conan-install.sh ${BUILD_TYPE} \
    -o "&:with_analyzer=False" \
    -o "&:with_python_module=False" \
    -o "&:with_square_detector=False" \
    -o "&:with_classifier_detector=False" \
    -o "&:with_barcode_decoder=False" \
    -o "&:with_pdf_input=False" \
    -o "&:with_signature_verifier=False"

${WORKSPACE_ROOT}/etc/cmake-config.sh ${BUILD_TYPE}
${WORKSPACE_ROOT}/build.sh ${BUILD_TYPE} ${@:2}

export PYTHONPATH=${WORKSPACE_ROOT}/build/${BUILD_TYPE}/bin
