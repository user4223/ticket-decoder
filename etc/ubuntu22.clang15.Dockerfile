# syntax=docker/dockerfile:1

FROM ubuntu:22.04

ARG TARGETARCH
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install --no-install-recommends -y cmake python-is-python3 python3-pip python3-dev libgtk2.0-dev wget

# Keep all commands above equal in all build container docker files to make layers re-usable

RUN apt-get install --no-install-recommends -y clang-15 libc++-15-dev libc++abi-15-dev lld-15 make

RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-15 800
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-15 800
RUN update-alternatives --install /usr/bin/ld.lld lld /usr/bin/ld.lld-15 800
RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang-15 800
RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-15 800
RUN update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld-15 800

RUN pip install conan==1.64.0 numpy
RUN conan profile new ticket-decoder --force --detect
RUN conan profile update settings.compiler=clang ticket-decoder
RUN conan profile update settings.compiler.version=15 ticket-decoder
RUN conan profile update settings.compiler.libcxx=libc++ ticket-decoder
RUN conan profile update conf.tools.system.package_manager:mode=install ticket-decoder
RUN ln -s /usr/bin/clang++ /usr/bin/g++ # Otherwise b2 is not building boost here because it uses hardwired g++ somewhere inside 

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
RUN mkdir -p cert && wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
COPY conanfile.py .
COPY etc/conan-install.sh etc/cmake-config.sh ./etc/cmake-build.sh etc/
# clang15 does not support armv8crypto intrinsics used by botan, so we have to disable for clang on arm
RUN echo $TARGETARCH
RUN etc/conan-install.sh Release $(if [ "$TARGETARCH" = "arm64" ]; then echo '-o botan:with_armv8crypto=False'; fi)
COPY <<EOF build.sh
    #!/usr/bin/env bash

    set -o errexit

    ./etc/cmake-config.sh Release
    ./etc/cmake-build.sh Release $@
EOF
RUN chmod 755 build.sh

ENV PYTHONPATH=/ticket-decoder/build/Release/bin
