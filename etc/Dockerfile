FROM ubuntu:focal

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install -y cmake python3-pip libgtk2.0-dev
RUN apt-get install -y clang-12 libc++-12-dev libc++abi-12-dev lld-12 

ENV CC=/usr/bin/clang-12
ENV CPP=/usr/bin/clang-cpp-12
ENV CXX=/usr/bin/clang++-12
ENV LD=/usr/bin/ld.lld-12

RUN pip install conan
RUN conan profile new default --detect
RUN conan profile update settings.compiler=clang default
RUN conan profile update settings.compiler.version=12 default

WORKDIR /aztec-detector
COPY source ./source
COPY setup.Release.sh ./setup.sh
COPY conanfile.txt ./conanfile.txt
COPY CMakeLists.txt ./CMakeLists.txt

RUN mkdir -p build/Release
WORKDIR /aztec-detector/build/Release
RUN conan install /aztec-detector/ -s build_type=Release --build missing
