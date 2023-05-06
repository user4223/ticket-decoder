[![build](https://github.com/karlheinzkurt/ticket-decoder/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/karlheinzkurt/ticket-decoder/actions/workflows/c-cpp.yml)

# Overview
Provide optimized and robust methods to detect and decode aztec codes by using opencv and zxing-cpp in combination with **signature validation** and to 
transcode UIC918 information into json structure.<br>
**Looking for build instructions? Take a look at the end of this document or check .github/workflows/c-cpp.yml!**

## ticket-decoder
Decoder can detect and decode aztec codes from given input images and prints json result on stdout or dumps it into file.

## ticket-analyzer
Analyzer is able to scan for aztec codes in images grabbed from camera or from a folder. It provides a simple interactive mode to visualize detection, image processing and decoding steps and to change some parameters to find optimal setup for detection. This application is considered to optimize default parameters and algorithms for the decoder.

## Considerations about optical Resolution
* Printed code size: 48mm (1.89inch)
* With 200dpi:       1.89 inch/code * 200 dot/inch ~ 380 dot/code
* With UIC-918-3:    380  dot       / 87 blocks    ~ 4.37 dot/block

# Record Documentation

## U_HEAD / U_TLAY / U_FLEX
* Recommendation on TAP TSI Revision - Technical Document - B12<br>
  https://www.era.europa.eu/system/files/2022-10/Recommendation%20on%20TAP%20TSI%20Revision%20-%20Technical%20Document%20-%20B12.pdf <br>
  (Link not working anymore? Go to https://www.era.europa.eu/ and search for "TAP TSI Revision - Technical Document - B12")<br>
  (TAP: Telematics Applications for Passenger Service)<br>
  (TSI: Technical Specifications for Interoperability)<br>
  
### Code generation for U_FLEX ASN.1 UPER
* UIC-barcode
  https://github.com/UnionInternationalCheminsdeFer/UIC-barcode (Apache License 2.0)
* Install free open source ANS.1 compiler (BSD 2)<br>
  https://github.com/vlm/asn1c<br>
  * MacOS: `brew install asn1c`
  * Ubuntu: `apt install -y asn1c`
* Generate code:
``` 
pushd etc
git clone https://github.com/UnionInternationalCheminsdeFer/UIC-barcode
popd

pushd source/lib/uic918/gen/v1.3
asn1c -fcompound-names -fwide-types -gen-PER ../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.3.asn
rm converter-sample.c
popd
```

## 0080VU
* Interoperabilität Barcode DB Online-Ticket<br>
  https://assets.static-bahn.de/dam/jcr:8fa0c0b5-d7b8-443b-b3cd-7ae902884847/236539-315207.pdf

## 0080BL
* Parser für Onlinetickets der Deutschen Bahn<br>
  https://github.com/rumpeltux/onlineticket (GPL 3.0)
* uic-918-3<br>
  https://github.com/justusjonas74/uic-918-3 (MIT)

# Signature Checking / Id-Mapping

* Public keys from UIC<br>
  https://railpublickey.uic.org/

* List of numeric codes for railway companies (RICS Code)<br>
  https://uic.org/IMG/pdf/codification_4n10jun22_publi.pdf

* DB Railway Station Documentation (EVA-Nummern)<br>
  https://data.deutschebahn.com/dataset/data-haltestellen.html

# Further Documentation and Ticket Samples
* Interoperability UIC/VDV codes, UIC918-3 and UIC918-9 example tickets and mappings for ids used in VDV codes<br>
  https://www.bahn.de/angebot/regio/barcode
  * [UIC918-3 Muster](https://assets.static-bahn.de/dam/jcr:c362849f-210d-4dbe-bb18-34141b5ba274/mdb_320951_muster-tickets_nach_uic_918-3_2.zip)
  * [UIC918-9 Muster](https://assets.static-bahn.de/dam/jcr:3c7a020a-7632-4f23-8716-6ebfc9f93ccb/Muster%20918-9.zip)

* DB-AGs OLT Barcode to VDV Data Structure Reference Implementation<br>
  https://sourceforge.net/projects/dbuic2vdvbc/

* HandyTicket-Fahrausweise des VRR im VDV-Barcode<br>
  https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf

* Additive Datenübertragung in Barcodes von internationalen Bahntickets<br>
  https://monami.hs-mittweida.de/frontdoor/deliver/index/docId/4983/file/WaitzRoman_Diplomarbeit.pdf

# Build Instructions
In general, when you want to avoid to install additional dependencies like non-default compilers and libraries on your system, consider using one of the build scripts using a docker container to create the build environment.<br>
As long as the conanfile.txt is unchanged, you can re-use the container with pre-built dependencies, source code changes are directly mirrored into build environment and artifacts are mirrored back into host system. In case dependencies change, the container gets re-build with updated dependencies.

* setup.ubuntu.clang12.Release.sh
* setup.ubuntu.gcc10.Release.sh

## Ubuntu focal with gcc10
```
apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -y \
                                       cmake git wget python3-pip libgtk2.0-dev \
                                       gcc-10 g++-10 cpp-10 libstdc++-10-dev
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 \
                    --slave /usr/bin/g++ g++ /usr/bin/g++-10

pip install conan==1.59.0
conan profile new --detect --force ticket-decoder
conan profile update settings.compiler=gcc ticket-decoder
conan profile update settings.compiler.version=10 ticket-decoder
conan profile update settings.compiler.libcxx=libstdc++11 ticket-decoder

git clone https://github.com/karlheinzkurt/ticket-decoder.git
cd ticket-decoder
./setup.Release.sh -j

wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
build/Release/bin/ticket-decoder-test

```

## Ubuntu focal with clang12
```
apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -y \
                                       cmake git wget python3-pip libgtk2.0-dev \
                                       clang-12 libc++-12-dev libc++abi-12-dev lld-12 

export CC=/usr/bin/clang-12
export CPP=/usr/bin/clang-cpp-12
export CXX=/usr/bin/clang++-12
export LD=/usr/bin/ld.lld-12

pip install conan==1.59.0
conan profile new --detect --force ticket-decoder
conan profile update settings.compiler=clang ticket-decoder
conan profile update settings.compiler.version=12 ticket-decoder
conan profile update settings.compiler.libcxx=libc++ ticket-decoder

git clone https://github.com/karlheinzkurt/ticket-decoder.git
cd ticket-decoder
./setup.Release.sh -j

wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
build/Release/bin/ticket-decoder-test

```

## MacOS with Apple clang14
```
xcode-select --install

pip install conan==1.59.0
conan profile new --detect --force ticket-decoder
conan profile update settings.compiler.version=14.0 ticket-decoder

git clone https://github.com/karlheinzkurt/ticket-decoder.git
cd ticket-decoder
./setup.Release.sh -j

wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
build/Release/bin/ticket-decoder-test
```
