[![build](https://github.com/karlheinzkurt/ticket-decoder/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/karlheinzkurt/ticket-decoder/actions/workflows/c-cpp.yml)

# ticket-decoder
Provide optimized and robust methods to detect and decode aztec codes by using opencv and zxing-cpp in combination and to 
transcode information into json structure.

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
* Install free open source ANS.1 compiler<br>
  https://github.com/vlm/asn1c<br>
  On macos by using: `brew install asn1c`
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

# Signature Checking / Id-Mapping / Specific Field Decoding 

* Public keys from UIC<br>
  https://railpublickey.uic.org/

* Interoperability UIC/VDV codes, UIC918-3 and UIC918-9 example tickets and mappings for ids used in VDV codes<br>
  https://www.bahn.de/angebot/regio/barcode
  
* HandyTicket-Fahrausweise des VRR im VDV-Barcode<br>
  https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf

* Additive Datenübertragung in Barcodes von internationalen Bahntickets<br>
  https://monami.hs-mittweida.de/frontdoor/deliver/index/docId/4983/file/WaitzRoman_Diplomarbeit.pdf

* List of numeric codes for railway companies (RICS Code)<br>
  https://uic.org/IMG/pdf/codification_4n10jun22_publi.pdf

* DB Railway Station Documentation (EVA-Nummern)<br>
  https://data.deutschebahn.com/dataset/data-haltestellen.html
