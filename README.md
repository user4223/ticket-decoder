[![build](https://github.com/karlheinzkurt/ticket-decoder/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/karlheinzkurt/ticket-decoder/actions/workflows/c-cpp.yml)

# Overview
Provide optimized and robust methods to detect and decode aztec codes by using opencv and zxing-cpp in combination with **signature validation** and to 
transcode UIC918 information into json structure. (UIC918-3 and UIC918-9)<br>

**Looking for build instructions? Take a look at the end of this document!**

## ticket-decoder
Decoder can detect and decode uic918 content from aztec codes from given input images/pdfs, verifies content and prints json result on stdout or dumps it into file.

Check `ticket-decoder --help` for arguments.

<img src="doc/images/decode_from_file.png" alt="ticket-decoder" height="250"/>
<p>

## ticket-analyzer
Analyzer is able to scan for aztec codes in images grabbed from camera or from images/pdfs in a folder. It provides a simple interactive mode to visualize detection, image processing and decoding steps and to change some parameters to find optimal setup for detection. This application is considered to optimize default parameters and algorithms for the decoder.

Check `ticket-analyzer --help` for arguments.

<img src="doc/images/analyze_from_file.png" alt="ticket-analyzer" height="250"/>
<p>

To get a minimal setup for experimentation, do the following:
* Download UIC918-3 and UIC918-9 sample tickets from https://www.bahn.de/angebot/regio/barcode and extract zip files into folder ./images/
* Download XML file containing public keys of issuers from https://railpublickey.uic.org/ into folder ./cert/ and name it UIC_PublicKeys.xml
* Run `./build/Release/bin/ticket-analyzer` from workspace folder or use arguments to specify different paths to input files and folders
* Use following keys to tweak settings:
  * i: Visualize next image processing step
  * I: Visualize previous image processing step
  * c: Visualize next contour detection step
  * C: Visualize previous contour detection step
  * f: Next image input file from image-folder
  * F: Previous image input file from image-folder
  *  : Toggle camera device (space)
  * r: Rotate image -1 degree
  * R: Rotate image +1 degree
  * 2: Split image into 2 parts and rotate over parts
  * 4: Split image into 4 parts and rotate over parts
  * s: Scale up image
  * S: Scale down image
  * 0: Reset: Rotation, Scale, Split
  * d: Rotate over available detector implementations
  * p: Assume pure barcode
  * b: Use local average binarizer
  * D: Dump current image into output-folder
  * o: Overlay detected barcode image
  * t: Overlay decoded content or text
* Check output-folder (./out by default) for intermediate images, raw data files or decoded data in json files

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

pushd source/lib/uic918/u_flex/v1.3/gen
asn1c -fcompound-names -fwide-types -gen-PER ../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v1.3.4.asn
rm converter-sample.c
pushd ../../v2.0/gen
asn1c -fcompound-names -fwide-types -gen-PER ../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v2.0.2.asn
rm converter-sample.c
pushd ../../v3.0/gen
asn1c -fcompound-names -fwide-types -gen-PER ../../../../../../etc/UIC-barcode/misc/uicRailTicketData_v3.0.3.asn
rm converter-sample.c
popd && popd && popd
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
  ```
  # You can use the following command to convert PDF file into images for further processing, but you don't have to because application is able to precess pdf files directly. But decoding quality might differ depending on parameters like DPI.
  # brew|apt install imagemagick
  convert -density 250 -trim -quality 100 -flatten <file name>.pdf <file name>.png
  ```

* DB-AGs OLT Barcode to VDV Data Structure Reference Implementation<br>
  https://sourceforge.net/projects/dbuic2vdvbc/

* HandyTicket-Fahrausweise des VRR im VDV-Barcode<br>
  https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf

* Additive Datenübertragung in Barcodes von internationalen Bahntickets<br>
  https://monami.hs-mittweida.de/frontdoor/deliver/index/docId/4983/file/WaitzRoman_Diplomarbeit.pdf

* KDE Barcode Formats - Ticket Barcode Formats<br>
  https://community.kde.org/KDE_PIM/KItinerary/Barcode_Formats
  Some details and collection of links related to different european rail companies and their ticket formats.


# Build Instructions

## Requirements
* gcc >= 11, clang >= 14 (other compilers and versions may work but are not tested)
* conan package manager < 2.0 (https://conan.io/)
* cmake >= 3.19

* python3 numpy (boost.python requires numpy for build and unfortunately, it is not possible to disable it via conan config)

Following libraries are used by the project. Usually you should not care about it since conan will do that for you.

* opencv        (image processing)
* zxing-cpp     (barcode/aztec-code decoding)
* nlohmann_json (json support - output)
* easyloggingpp (logging)
* pugixml       (xml support - public key file)
* botan         (signature verification)
* tclap         (cli argument processing)
* gtest         (unit testing)
* poppler       (pdf reading/rendering)
* boost.python  (python binding)

## Ubuntu jammy

### Inside docker build container
In general, when you want to avoid to install additional dependencies like non-default compilers and libraries on your system, consider using one of the build scripts using a docker container to create the build environment.<br>
As long as the conanfile.py is unchanged, you can re-use the container with pre-built dependencies, source code changes are directly mirrored into build environment and artifacts are mirrored back into host system. In case dependencies change, the container gets re-build with updated dependencies.

**-> this will install dependencies and run the build inside a ubuntu docker container**

* setup.ubuntuJammy.gcc11.Release.sh
* setup.ubuntuJammy.clang15.Release.sh

When the preparation of the build environment has been successful, it should be possible to build the project by using `./build.sh -j` **inside the build container**.

Take a look into `./build/` folder to discover artifacts. You should be able to execute the executables on host machine as well.

### On host machine
When opencv has to be built from source because of missing pre-built package for your arch/os/compiler mix, it might 
be necessary to install some further xorg/system libraries to make highgui stuff building inside conan install process. 
To get this handled properly, use the following conan config flags:
* conf.tools.system.package_manager:mode=install
* conf.tools.system.package_manager:sudo_askpass=True

as shown below OR install ALL required xorg dependencies manually.
For details about specific required packages please check the error message carefully or see
the step "Install compiler and stdlib" in ".github/workflows/c-cpp.yml" for a list of dev-package names.
```
apt-get install --no-install-recommends -y build-essential cmake git python-is-python3 python3-pip python3-dev libgtk2.0-dev wget

pip3 install conan==1.64.0 numpy
conan profile new --detect --force ticket-decoder
conan profile update settings.compiler.libcxx=libstdc++11 ticket-decoder
conan profile update conf.tools.system.package_manager:mode=install ticket-decoder
conan profile update conf.tools.system.package_manager:sudo_askpass=True ticket-decoder

git clone https://github.com/karlheinzkurt/ticket-decoder.git
cd ticket-decoder
./setup.Release.sh -j

wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
build/Release/bin/ticket-decoder-test
```

## MacOS with Apple clang15 (amd64 & arm64)
t might be required for dependencies to get built properly during conan install to have a 
`python` command (without 3) in path available. So when you face an error like `python: command not found`
it might be required to create a link via `sudo ln -s $(which python3) /usr/local/bin/python` since there
is no package python-is-python3 in homebrew available, as it is for ubuntu.
```
xcode-select --install

brew install cmake
pip3 install conan==1.64.0 numpy
conan profile new --detect --force ticket-decoder
conan profile update settings.compiler.version=15.0 ticket-decoder

git clone https://github.com/karlheinzkurt/ticket-decoder.git
cd ticket-decoder
./setup.Release.sh -j

wget 'https://railpublickey.uic.org/download.php' -O cert/UIC_PublicKeys.xml
build/Release/bin/ticket-decoder-test
```

## Windows

For sure, it should be possible to get it built by using visual compiler and toolchain as well. But I never tried and you might need to modify some build parameters/arguments and you have know (or to find out) how to setup toolchain, conan and cmake in Windows environment. Furthermore, the compiler might complain about things gcc and clang are not complaining about. But when you are an experienced dev, you should be able to get it managed. (support of multiple u_flex versions via asn1c generated and unprefixed C source files in a shared lib makes this a bit harder, most probably, since export/import of shared libs has to be ported to visual compiler world to, but it's possible via crazy macro stuff, i know) 
