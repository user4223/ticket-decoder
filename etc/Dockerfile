FROM ubuntu:focal

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install -y cmake python3-pip libgtk2.0-dev
RUN apt-get install -y clang-12 libc++-12-dev libc++abi-12-dev lld-12 

ENV CC=/usr/bin/clang-12
ENV CPP=/usr/bin/clang-cpp-12
ENV CXX=/usr/bin/clang++-12
ENV LD=/usr/bin/ld.lld-12
# ENV CMAKE_CXX_FLAGS=-stdlib=libc++ # Not working this way

RUN pip install conan
RUN conan profile new default --detect
RUN conan profile update settings.compiler=clang default
RUN conan profile update settings.compiler.version=12 default
RUN conan profile update settings.compiler.libcxx=libc++ default

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
COPY conanfile.txt ./conanfile.txt

WORKDIR /ticket-decoder/build/Release
RUN conan install /ticket-decoder/ -s build_type=Release --build missing

WORKDIR /ticket-decoder
### gets mounted into container to reflect runtime changes directly
# COPY setup.Release.sh ./setup.sh
# COPY CMakeLists.txt ./CMakeLists.txt
# COPY source ./source

# RUN cmake -S /ticket-decoder/ -B /ticket-decoder/build/Release/ -DCMAKE_BUILD_TYPE=Release
# RUN cmake --build /ticket-decoder/build/Release/ --config Release
