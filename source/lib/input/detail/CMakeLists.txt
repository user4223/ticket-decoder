PROJECT(ticket-decoder-input-detail)

add_subdirectory(api)
add_subdirectory(image)
IF (WITH_PDF_INPUT)
    add_subdirectory(pdf)
ENDIF()
