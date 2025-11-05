PROJECT(ticket-decoder-lib)

add_subdirectory(infrastructure)

add_subdirectory(io)
add_subdirectory(barcode)
add_subdirectory(dip)

add_subdirectory(interpreter)
add_subdirectory(utility)

IF (WITH_TICKET_ANALYZER)
    add_subdirectory(ui)
ENDIF()

add_subdirectory(api)
