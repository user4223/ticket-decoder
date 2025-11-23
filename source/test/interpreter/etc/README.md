# Decode raw data of sample tickets
* PDFs download: https://www.bahn.de/angebot/regio/barcode
* Install ImageMagick: `brew install imagemagick`
* Conversion to image: `convert -verbose -density 250 -trim images/ticket.pdf -quality 100 -flatten source/test/uic918/etc/ticket.png`
* Execute app for image with dump option enabled to extract raw data file
* Use raw files for uic918 decoding tests without side effects
