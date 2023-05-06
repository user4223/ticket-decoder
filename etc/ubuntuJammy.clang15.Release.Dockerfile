FROM ubuntu:jammy

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get -y upgrade && apt-get clean
RUN apt-get install --no-install-recommends -y cmake python-is-python3 python3-pip libgtk2.0-dev wget
RUN apt-get install --no-install-recommends -y clang-15 libc++-15-dev libc++abi-15-dev lld-15 

ENV CC=/usr/bin/clang-15
ENV CPP=/usr/bin/clang-cpp-15
ENV CXX=/usr/bin/clang++-15
ENV LD=/usr/bin/ld.lld-15

RUN pip install conan==1.59.0
RUN conan profile new ticket-decoder --force --detect
RUN conan profile update settings.compiler=clang ticket-decoder
RUN conan profile update settings.compiler.version=15 ticket-decoder
RUN conan profile update settings.compiler.libcxx=libc++ ticket-decoder

RUN mkdir -p /ticket-decoder/build/Release
WORKDIR /ticket-decoder
COPY conanfile.txt ./conanfile.txt
#RUN conan install -if build/Release -pr ticket-decoder -pr:b ticket-decoder -s build_type=Release --build missing .

#RUN mkdir -p cert && wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
