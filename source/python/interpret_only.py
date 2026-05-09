# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

from ticket_decoder import DecoderFacade
from zxingcpp import read_barcodes
from cv2 import imread

image = imread('source/test/decoder/etc/Muster 918-9 Länderticket Schleswig-Holstein_0_decoded.jpg')
barcodes = read_barcodes(image)
if not barcodes:
    print("No barcodes found")
    exit(1)

decoder_facade = DecoderFacade(\
    fail_on_interpreter_error = False,\
    uic_public_key_xml_file = "cert/UIC_PublicKeys.xml",\
    vdv_certificate_ldif_file = "cert/VDV_Certificates.ldif")

print(decoder_facade.decode_bytes(barcodes[0].bytes))
