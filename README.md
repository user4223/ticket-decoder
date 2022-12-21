[![build](https://github.com/karlheinzkurt/ticket-decoder/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/karlheinzkurt/ticket-decoder/actions/workflows/c-cpp.yml)

# ticket-decoder
Provide optimized and robust methods to detect and decode aztec codes by using opencv and zxing-cpp in combination.

* with code size: 48mm (1.89inch)
* with 200dpi:    1.89 inch/code * 200 dot/inch ~ 380 dot/code
* with UIC-918-3: 380  dot       / 87 blocks    ~ 4.37 dot/block

# Documentation

## Records 
### U_HEAD / U_TLAY / U_FLEX
* Recommendation on TAP TSI Revision - Technical Document - B12<br>
  https://www.era.europa.eu/system/files/2022-10/Recommendation%20on%20TAP%20TSI%20Revision%20-%20Technical%20Document%20-%20B12.pdf <br>
  (Not working anymore? Go to https://www.era.europa.eu/ and enter "TAP TSI Revision - Technical Document - B12" into search dialog)
  
#### Code generation for U_FLEX ASN.1 UPER
* Retrieve newest barcode.asn<br>
  https://www.era.europa.eu/sites/default/files/barcode.zip (fetched 2022-08-19)
* Unzip barcode.zip to barcode.asn to make it accessable for asn compiler command
* Install free open source ANS.1 compiler<br>
  https://github.com/vlm/asn1c<br>
  On macos by using: `brew install asn1c`
* Generate code:
``` 
pushd source/lib/uic918/gen 
asn1c -fcompound-names -fwide-types -gen-PER ../../../doc/barcode.asn
popd popd
```

### 0080VU
* Interoperabilität Barcode DB Online-Ticket<br>
  https://assets.static-bahn.de/dam/jcr:8fa0c0b5-d7b8-443b-b3cd-7ae902884847/236539-315207.pdf

### 0080BL
* Parser für Onlinetickets der Deutschen Bahn<br>
  https://github.com/rumpeltux/onlineticket (GPL 3.0)
* uic-918-3<br>
  https://github.com/justusjonas74/uic-918-3 (MIT)

### Signature Checking / Id-Mapping / Specific Field Decoding 

* Public keys from UIC<br>
  https://railpublickey.uic.org/

* Interoperability UIC/VDV codes, UIC918-3 and UIC918-9 example tickets and mappings for ids used in VDV codes<br>
  https://www.bahn.de/angebot/regio/barcode
  
* HandyTicket-Fahrausweise des VRR im VDV-Barcode<br>
  https://www.kcd-nrw.de/fileadmin/03_KC_Seiten/KCD/Downloads/Technische_Dokumente/Archiv/2010_02_12_kompendiumvrrfa2dvdv_1_4.pdf

* List of numeric codes for railway companies (RICS Code)<br>
  https://uic.org/IMG/pdf/codification_4n10jun22_publi.pdf

* DB Railway Station Documentation (EVA-Nummern)<br>
  https://data.deutschebahn.com/dataset/data-haltestellen.html
