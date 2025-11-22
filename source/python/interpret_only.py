from ticket_decoder import DecoderFacade
from zxingcpp import read_barcodes
from cv2 import imread
from base64 import b64encode

image = imread('source/test/decoder/etc/Muster 918-9 Länderticket Schleswig-Holstein_0_decoded.jpg')
barcodes = read_barcodes(image)
if not barcodes:
    print("No barcodes found")
    exit(1)

decoder_facade = DecoderFacade(fail_on_interpreter_error = False, public_key_file = "cert/UIC_PublicKeys.xml")
print(decoder_facade.decode_uic918(b64encode(barcodes[0].bytes)))
