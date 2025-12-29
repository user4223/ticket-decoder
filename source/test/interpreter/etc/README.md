<!--
SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
SPDX-License-Identifier: GPL-3.0-or-later
-->

# Conversion from PDF to image file
This is not required since ticket-decoder is able to read PDF files directly. 
Anyway, there might be use cases it's necessary to do so.

* PDFs download: https://www.bahn.de/angebot/regio/barcode
* Install ImageMagick: `brew install imagemagick` or `apt -y install imagemagick`
* Conversion to image: `convert -verbose -density 250 -trim images/ticket.pdf -quality 100 -flatten images/ticket.png`

# Raw data extraction
Be careful and avoid to publish personal information hidden in binary data here. Use official 
sample data only.

* Execute ticket-decoder for image with raw-dump option enabled to extract base64 encoded raw data `ticket-decoder -i "./images/ticket.pdf" -R`
* Copy base64 encoded raw data for uic918 decoding tests without side effects
