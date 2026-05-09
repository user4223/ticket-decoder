[![pypi](https://github.com/user4223/ticket-decoder/actions/workflows/pypi.yml/badge.svg)](https://github.com/user4223/ticket-decoder/actions/workflows/pypi.yml)


# ticket_decoder

Optional arguments for instance creation of DecoderFacade.
* `uic_public_key_xml_file` = "cert/UIC_PublicKeys.xml"  
  Relative path to the XML file containing UIC public keys you can obtain here: https://railpublickey.uic.org/ .
* `vdv_certificate_ldif_file` = "cert/VDV_Certificates.ldif"  
  Relative path to the LDIF file containing VDV certificates to decrypt VDV barcode content.
* `fail_on_decoder_error` = false  
  Fail when the aztec/barcode decoder gets an error if true or deliver an empty result when false.
* `fail_on_interpreter_error` = true  
  Fail when the interpretation of the data gets an error if true or deliver an empty result when false.

Provided python API is in an early state and the class DecoderFacade supports 3 methods right now only.
* `decode_bytes('...')`
* `decode_base64('...')` is considered for the use case you decode the raw data from aztec-code
  in advance via zxing-cpp or other aztec-code-decoding library of your choice and you want to decode
  raw train ticket barcode data to json only.
  In this case, be careful with the output of the decoder to avoid string encodings like UTF8
  or other multi-byte encodings. Ideally try to get access to the raw byte array and just
  encode those bytes to base64 before passing it to the method.
  If your aztec-code-decoder provides a string-type only and you are able to pass a character-encoding
  option, try using 'ISO 8859-1' and cast the result string to raw bytes.

* `decode_files('...')` detects and decodes aztec-codes from file or directories (PDF, image) and decodes barcode
  data to json. This is using zxing-cpp internally. It returns an array of tuples (input-path and json-result) 
  of size x, while x is the amount of aztec-codes found on input. An image can contain always multiple barcodes,
  the the return value is alway a mapping of origin to content.

## Example decoding directly from PDF or image
```
from ticket_decoder import DecoderFacade

decoder_facade = DecoderFacade(fail_on_interpreter_error = False)
for result in decoder_facade.decode_files('path/2/your/ticket.pdf'):
   print(result[1])
```

## Decoding Aztec-Code in advance using zxingcpp on Python side
```
from ticket_decoder import DecoderFacade
from zxingcpp import read_barcodes
from cv2 import imread
from base64 import b64encode

image = imread('path/2/your/ticket.jpg')

barcodes = read_barcodes(image)
if not barcodes:
    print("No barcodes found")
    exit(1)

decoder_facade = DecoderFacade(\
    fail_on_interpreter_error = False,\
    uic_public_key_xml_file = "cert/UIC_PublicKeys.xml",\
    vdv_certificate_ldif_file = "cert/VDV_Certificates.ldif")

print(decoder_facade.decode_base64(b64encode(barcodes[0].bytes)))
```
