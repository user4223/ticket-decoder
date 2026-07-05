# syntax=docker/dockerfile:1
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

FROM ubuntu:24.04

ARG CLANG_VERSION
ARG TARGETARCH
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y --no-install-recommends make cmake wget git python-is-python3 python3-pip python3-venv
# Keep all commands above equal in all build container docker files to make layers re-usable
RUN apt-get install -y --no-install-recommends clang-$CLANG_VERSION libc++-$CLANG_VERSION-dev libc++abi-$CLANG_VERSION-dev lld-$CLANG_VERSION libgtk2.0-dev python3-dev
RUN apt-get clean

RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-$CLANG_VERSION 800
RUN update-alternatives --install /usr/bin/g++     g++     /usr/bin/clang++-$CLANG_VERSION 800
RUN update-alternatives --install /usr/bin/c++     c++     /usr/bin/clang++-$CLANG_VERSION 800
RUN update-alternatives --install /usr/bin/clang   clang   /usr/bin/clang-$CLANG_VERSION   800
RUN update-alternatives --install /usr/bin/gcc     gcc     /usr/bin/clang-$CLANG_VERSION   800
RUN update-alternatives --install /usr/bin/cc      cc      /usr/bin/clang-$CLANG_VERSION   800
RUN update-alternatives --install /usr/bin/ld.lld  lld     /usr/bin/ld.lld-$CLANG_VERSION  800
RUN update-alternatives --install /usr/bin/ld      ld      /usr/bin/ld.lld-$CLANG_VERSION  800

WORKDIR /ticket-decoder
COPY etc/conan-config.sh etc/conan-install.sh etc/cmake-config.sh etc/cmake-build.sh etc/python-test.sh etc/
COPY etc/poppler/ etc/poppler
COPY etc/conan/profiles etc/conan/profiles
COPY cert/install-uic-keys.sh cert/install-vdv-certificates.sh cert/
COPY .env.default .env
COPY requirements.txt conanfile.py setup.WASM.sh ./

RUN python3 -m venv venv
ENV PATH="venv/bin:$PATH"
RUN pip install -r requirements.txt

RUN etc/conan-config.sh clang $CLANG_VERSION

RUN echo etc/conan-install.sh Release \
    -o:a="&:with_ticket_analyzer=False" \
    -o:a="&:with_ticket_decoder=False" \
    -o:a="&:with_python_module=False" \
    -o:a="&:with_wasm_module=True" \
    -o:a="&:with_square_detector=False" \
    -o:a="&:with_classifier_detector=False" \
    -o:a="&:with_barcode_decoder=True" \
    -o:a="&:with_pdf_input=False" \
    -o:a="&:with_signature_verifier=True" \
    -o:a="&:with_uic_interpreter=True" \
    -o:a="&:with_vdv_interpreter=True" \
    -o:a="&:with_sbb_interpreter=True" \
    -pr:h="./etc/conan/profiles/emscripten" \
    -o:a='botan/*:amalgamation=False' \
    -s:b='compiler=emcc'

#-c:b='opencv/*:tools.build:defines=["WASM=1","CV_ENABLE_INTRINSICS=OFF","__EMSCRIPTEN_MAJOR__=3","__EMSCRIPTEN_MINOR__=1","__EMSCRIPTEN_TINY__=73"]'
#-c:b='opencv/*:tools.build:cxxflags=["-msimd128","-mrelaxed-simd"]'

# COPY <<EOF build.sh
#     #!/usr/bin/env bash
# 
#     set -o errexit
# 
#     ./etc/cmake-config.sh Release
#     ./etc/cmake-build.sh Release \$\@
# EOF
# RUN chmod 755 build.sh
# 
# RUN cert/install-uic-keys.sh
# RUN cert/install-vdv-certificates.sh
