# syntax=docker/dockerfile:1
# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

FROM ubuntu:24.04

ARG GCC_VERSION
ARG TARGETARCH
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y --no-install-recommends make cmake wget git python-is-python3 python3-pip python3-venv
# Keep all commands above equal in all build container docker files to make layers re-usable
RUN apt-get install -y --no-install-recommends gcc-$GCC_VERSION g++-$GCC_VERSION cpp-$GCC_VERSION libstdc++-$GCC_VERSION-dev pkg-config
RUN apt-get clean

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-$GCC_VERSION 800
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-$GCC_VERSION 800
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-$GCC_VERSION 800
RUN update-alternatives --install /usr/bin/cc  cc  /usr/bin/gcc-$GCC_VERSION 800

WORKDIR /ticket-decoder
COPY etc/python-test.sh etc/
COPY etc/poppler/ etc/poppler
COPY etc/conan/profiles etc/conan/profiles
COPY cert/install-uic-keys.sh cert/install-vdv-certificates.sh cert/
COPY LICENSES/ LICENSES
COPY README.md pyproject.toml conanfile.py source/python/requirements.txt ./

RUN pip install uv --break-system-packages && uv venv ./venv
ENV PATH="venv/bin:$PATH"

COPY <<EOF build.sh
    #!/usr/bin/env bash

    set -o errexit
    
    uv build --wheel
    uv pip install --upgrade dist/ticket_decoder*.whl
EOF
RUN chmod 755 build.sh

RUN uv pip install ldap3 -r requirements.txt

RUN cert/install-uic-keys.sh
RUN cert/install-vdv-certificates.sh
