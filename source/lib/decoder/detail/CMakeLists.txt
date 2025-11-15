PROJECT(ticket-decoder-decoder-detail)

IF (WITH_BARCODE_DECODER)
    add_subdirectory(zxing)
ENDIF()
