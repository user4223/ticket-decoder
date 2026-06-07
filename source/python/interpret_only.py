# SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
# SPDX-License-Identifier: GPL-3.0-or-later

from ticket_decoder import DecoderFacade
from zxingcpp import read_barcodes
from cv2 import imread

file = 'source/test/decoder/etc/Muster 918-9 Länderticket Schleswig-Holstein_0_decoded.jpg'

image = imread(file)
if image is None:
    print("Image not found")
    exit(1)

barcodes = read_barcodes(image)
if barcodes is None:
    print("No barcodes found")
    exit(1)

decoder_facade = DecoderFacade()

print(decoder_facade.decode_bytes(barcodes[0].bytes, file))
