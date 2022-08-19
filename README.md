[![build](https://github.com/karlheinzkurt/aztec-detector/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/karlheinzkurt/aztec-detector/actions/workflows/c-cpp.yml)

# aztec-detector
Provide optimized and robust methods to detect and decode aztec codes by using opencv and zxing-cpp in combination.

* with code size: 48mm (1.89inch)
* with 200dpi:    1.89 inch/code * 200 dot/inch ~ 380 dot/code
* with UIC-918-3: 380  dot       / 87 blocks    ~ 4.37 dot/block

# Documentation
* Recommendation of European Railway Agency:
  * https://www.era.europa.eu/sites/default/files/library/docs/recommendation/era_rec122_tap_tsi_revision_recommendation_technical_document_b12_en.pdf

* Certificates, examples and documentation of specific non-UIC records Deutsche Bahn:
  * https://www.bahn.de/angebot/regio/barcode
  * https://assets.static-bahn.de/dam/jcr:8fa0c0b5-d7b8-443b-b3cd-7ae902884847/236539-315207.pdf
  * https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf

* RICS codes:
  * https://uic.org/IMG/pdf/codification_4n10jun22_publi.pdf

# FLEX ASN.1
## Descriptions / Module
* https://www.era.europa.eu/library/era-recommendations_en
* https://www.era.europa.eu/sites/default/files/barcode.zip

Unzip barcode.zip to doc/barcode.asn to make it accessable for asn compiler command below.

## ASN.1 UPER decoding code generation

Using free open source ANS.1 compiler: https://github.com/vlm/asn1c

```
 brew install asn1c
 
 pushd source/lib/uic918/gen 
 asn1c -fcompound-names -fwide-types -gen-PER ../../../../doc/barcode.asn
 popd popd
```
